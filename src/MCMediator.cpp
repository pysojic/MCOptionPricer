// MCMediator.cpp
// 
// Implementation of mediator
// 
// Pierre-Yves Sojic
//


#ifndef MCMEDIATOR_CPP
#define MCMEDIATOR_CPP

#include <algorithm>
#include <atomic>
#include <chrono>
#include <execution>
#include <mutex>
#include <ranges>
#include <tuple>

#include "MCMediator.hpp"
#include "StopWatch.hpp"

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
    std::for_each(std::execution::par_unseq, iota.begin(), iota.end(), [&](std::size_t i)
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
                    ++j; // Advance to the next time step
                });
            // Send path data to the Pricers
            m_path(m_res);
        });
    sw.Stop();

    // Inform pricer to finish, pass the duration of the process
    m_finish(sw.GetTime());
}

#endif // !MCMEDIATOR_CPP