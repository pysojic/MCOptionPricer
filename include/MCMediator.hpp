// Mediator.hpp
// 
// Mediator used to orchestrate the different part of the MC Simulator
// 
// Pierre-Yves Sojic
//

#pragma once

#include <functional>
#include <mutex>

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

#ifndef MCMEDIATOR_CPP
#include "MCMediator.cpp"
#endif // !MCMEDIATOR_CPP
