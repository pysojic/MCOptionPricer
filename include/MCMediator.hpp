// Mediator.hpp
// 
// Mediator used to orchestrate the different part of the MC Simulator
// 
// Pierre-Yves Sojic
//

#pragma once

#include <functional>
#include <algorithm>
#include <atomic>
#include <chrono>
#include <execution>
#include <mutex>
#include <ranges>
#include <tuple>

#include "StopWatch.hpp"
#include "SDEBase.hpp"
#include "FDMAbstract.hpp"
#include "RNGAbstract.hpp"

template<typename SDE>
class MCMediator
{
public:
    using FDMPointer = std::unique_ptr<FDMAbstract<SDE>>;
    using RNGPointer = std::unique_ptr<RNGAbstract>;
    using PartsTuple = std::tuple<SDEBase<SDE>, FDMPointer, RNGPointer>;
    using OptionPath = std::function<void(const std::vector<double>& path)>;
    using Finish = std::function<void(double)>;
    using NSimDisplay = std::function<void(std::size_t)>;

public:
    MCMediator(PartsTuple& parts, const OptionPath& optionPath, const Finish& finish, std::size_t numberSimulations);

    void start();

private:
    // Three main components
    SDEBase<SDE> m_sde;
    FDMPointer m_fdm;
    RNGPointer m_rng;
    // Other MC-related data 
    std::size_t m_NSim;         // Number of simulations
    std::vector<double> m_res;  // Generated path
    OptionPath m_path;          // Function that sends the generated path to the pricer
    Finish m_finish;            // Function that notifies the pricer to finish and output the option price
    NSimDisplay m_mis;          // Function to display the count of simulations
    std::mutex m_mutex;
};


template <typename SDE>
MCMediator<SDE>::MCMediator(PartsTuple& parts, const OptionPath& optionPath, const Finish& finish, std::size_t numberSimulations)
    : m_sde(std::get<0>(parts)), m_fdm(std::move(std::get<1>(parts))), m_rng(std::move(std::get<2>(parts))), m_NSim(numberSimulations),
    m_res(m_fdm->get_NT() + 1, 0), m_path(optionPath), m_finish(finish)
{
    m_mis = [](std::size_t i)
        {
            if (i % 10000 == 0)
            {
                std::cout << "\rSimulation Count: #" << i << std::flush; // Use \r to overwrite the line
            }
        };
}

template <typename SDE>
void MCMediator<SDE>::start()
{
    StopWatch sw;

    sw.Start();

    m_res[0] = m_sde.initial_condition();

    // Usina a iota range to iterate over in the for_each loop
    auto iota = std::ranges::views::iota(1, (int)m_NSim + 1);
    std::for_each(std::execution::par, iota.begin(), iota.end(), [&](std::size_t i)
        { // Calculate a path at each iteration
         //   if ((i / 5000) * 5000 == i)
          // Give status after a given numbers of iterations
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                m_mis(i);
            }

            std::atomic_size_t j = 1;
            std::for_each(m_res.begin() + 1, m_res.end(), [&](double a)
                {
                    // Compute the solution at level n+1
                    m_res[j] = m_fdm->advance(m_res[j - 1], m_fdm->get_mesh()[j - 1], m_fdm->get_meshSize(), m_rng->generate_rn(), m_rng->generate_rn());
                    j.fetch_add(1, std::memory_order_relaxed); // Advance to the next time step
                });
            // Send path data to the Pricers
            m_path(m_res);
            
        });
    sw.Stop();

    // Inform pricer to finish, pass the duration of the process
    m_finish(sw.GetTime());
}
