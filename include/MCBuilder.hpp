// MCBuilder.hpp
// 
// Builder used to assemble the type of option/sde.
// 
// Pierre-Yves Sojic
//

#pragma once

#include <memory>
#include <tuple>
#include <functional>
#include <iostream>
#include <exception>

#include "OptionData.hpp"
#include "SDEBase.hpp"
#include "SDEConcrete.hpp"
#include "FDMAbstract.hpp"
#include "FDMDerived.hpp"
#include "PricerAbstract.hpp"
#include "PricerDerived.hpp"
#include "RNGAbstract.hpp"
#include "RNGDerived.hpp"

template <typename SDE>
class MCBuilder
{
public:
    using FDMPointer = std::unique_ptr<FDMAbstract<SDE>>;
    using RNGPointer = std::unique_ptr<RNGAbstract>;
    using PricerPointer = std::shared_ptr<PricerAbstract>;
    using PartsTuple = std::tuple<SDEBase<SDE>, FDMPointer, RNGPointer>;
    using OptionPath = std::function<void(const std::vector<double>& path)>;
    using Finish = std::function<void(double)>;

public:
    MCBuilder(const std::shared_ptr<OptionData>& optionData);

    PartsTuple parts(const SDEBase<SDE>& sde, const FDMAbstract<SDE>& fdm, const RNGAbstract& rng) const;
    PartsTuple parts(); // Takes user input in the console for the different option
    OptionPath get_path() const;
    Finish get_finish() const;

private:
    SDEBase<SDE> get_SDE() const;
    FDMPointer get_FDM(const SDEBase<SDE>& sde) const;
    RNGPointer get_RNG() const;
    PricerPointer get_pricer();

private:
    std::shared_ptr<OptionData> m_data; // Option data
    OptionPath m_path;                  // Function used to generate the path
    Finish m_finish;                    // Function used to signal pricer to wrap up
}; 

//--------------Default Builder-----------------

template <typename SDE>
class MCDefaultBuilder
{
public:
    using FDMPointer = std::unique_ptr<FDMAbstract<SDE>>;
    using RNGPointer = std::unique_ptr<RNGAbstract>;
    using PricerPointer = std::shared_ptr<PricerAbstract>;
    using PartsTuple = std::tuple<SDEBase<SDE>, FDMPointer, RNGPointer>;
    using OptionPath = std::function<void(const std::vector<double>& path)>;
    using Finish = std::function<void(double)>;

public:
    MCDefaultBuilder(const std::shared_ptr<OptionData>& optionData);

    PartsTuple parts(); // Takes user input in the console for the different param
    OptionPath get_path() const;
    Finish get_finish() const;

private:
    SDEBase<SDE> get_SDE() const;
    FDMPointer get_FDM(const SDEBase<SDE>& sde) const;
    RNGPointer get_RNG() const;
    PricerPointer get_pricer();

private:
    std::shared_ptr<OptionData> m_data; // Option data
    OptionPath m_path;                  // Function used to generate the path
    Finish m_finish;                    // Function used to signal pricer to wrap up
};

// ---------------Implementations---------------

template <typename SDE>
MCBuilder<SDE>::MCBuilder(const std::shared_ptr<OptionData>& optionData)
	: m_data{ optionData }, m_path{ nullptr }, m_finish { nullptr }
{}

template <typename SDE>
MCBuilder<SDE>::PartsTuple MCBuilder<SDE>::parts(const SDEBase<SDE>& sde, const FDMAbstract<SDE>& fdm, const RNGAbstract& rng) const
{
	return std::make_shared<MCBuilder<SDE>::PartsTuple>(
		std::make_unique<SDEBase<SDE>>(sde),
		std::make_unique<FDMAbstract>(fdm),
		std::make_unique<RNGAbstract>(rng));
}

template <typename SDE>
MCBuilder<SDE>::PartsTuple MCBuilder<SDE>::parts()
{
    SDEBase<SDE> sde = std::move(get_SDE());
	FDMPointer fdm = std::move(get_FDM(sde));
	RNGPointer rng = std::move(get_RNG());
	PricerPointer pricer = std::move(get_pricer());

    return std::make_tuple(std::move(sde), std::move(fdm), std::move(rng));
}

template <typename SDE>
MCBuilder<SDE>::OptionPath MCBuilder<SDE>::get_path() const
{
    return m_path;
}

template <typename SDE>
MCBuilder<SDE>::Finish MCBuilder<SDE>::get_finish() const
{
    Interface::instance()->m_data = m_data;
    return m_finish;
}

template <typename SDE>
SDEBase<SDE> MCBuilder<SDE>::get_SDE() const
{
	if constexpr (std::is_same_v<SDE, GBM>)
	{ // GBM
		return SDEBase<SDE>(GBM(m_data));
	}
	else
	{
		return SDEBase<SDE>(CEV(m_data));
	}
}

template <typename SDE>
MCBuilder<SDE>::FDMPointer MCBuilder<SDE>::get_FDM(const SDEBase<SDE>& sde) const
{
    enum class FDMChoice
    {
        Euler = 1,
        Exact,
        
        FINISH // Add an addition choice that throw errors if chosen or above
    };

    std::cout << "Create FDM:\n";
    std::cout << "Choose a FDM: 1. Euler, 2. Exact\n";

	short choice;
    std::cin >> choice;
    FDMChoice FDMchoice = static_cast<FDMChoice>(choice);

    if(FDMchoice >= FDMChoice::FINISH || FDMchoice < FDMChoice::Euler)
        throw std::invalid_argument("Invalid FDM. Make sure to enter a valid number.");

    std::cout << "How many time subdivisions for the FDM?\n";
    long NT;
    std::cin >> NT;

    if (NT < 1)
        throw std::invalid_argument("NT must be a stricly positive integer.");

    switch (FDMchoice)
    {
    case FDMChoice::Euler:
        return std::make_unique<EulerFDM<SDE>>(sde, NT);

    case FDMChoice::Exact:
        return std::make_unique<ExactFDM<SDE>>(sde, NT, m_data->S0, m_data->vol, m_data->r);
        
    default:
        return nullptr;
    }
}

template <typename SDE>
MCBuilder<SDE>::RNGPointer MCBuilder<SDE>::get_RNG() const
{
    enum class RNGChoice
    {
        MersenneTwister = 1,
        PolarMarsagliaNet,
        BoxMuller
    };

    std::cout << "Create RNG:\n";
    std::cout << "Choose a RNG: 1. MersenneTwister, 2. PolarMarsagliaNet, 3. Box-Muller\n";

    short choice;
    std::cin >> choice;
    RNGChoice RNGchoice = static_cast<RNGChoice>(choice);
    
    switch (RNGchoice)
    {
    case RNGChoice::MersenneTwister:
        return std::make_unique<MersenneTwister>();

    case RNGChoice::PolarMarsagliaNet:
        return std::make_unique<PolarMarsagliaNet>();

    case RNGChoice::BoxMuller:
        return std::make_unique<BoxMuller>();

    default:
        throw std::invalid_argument("Invalid RNG. Make sure you enter a valid number.");
    }
}

template <typename SDE>
MCBuilder<SDE>::PricerPointer MCBuilder<SDE>::get_pricer()
{
    enum class PricerChoice
    {
        European = 1,
        Asian, 
        Barrier
    };

    std::cout << "Create Pricer:\n";
    std::cout << "Choose an option pricer: 1. European, 2. Asian, 3. Barrier\n";
    unsigned short choice;
    std::cin >> choice;
    PricerChoice pricerChoice = static_cast<PricerChoice>(choice);

    auto callPayoff = [this](double a) { return std::max(a - m_data->K, 0.0); };
    auto putPayoff = [this](double a) { return std::max(m_data->K - a, 0.0); };
    auto discounter = [this]() { return std::exp(-m_data->r * m_data->T); };
    PricerPointer p = nullptr;

    switch (pricerChoice)
    {
    case PricerChoice::European:
        p = std::make_shared<EuropeanPricer>(callPayoff, putPayoff, discounter, 0);
        break;

    case PricerChoice::Asian:
        p = std::make_shared<AsianPricer>(callPayoff, putPayoff, discounter, 0);
        break;

    case PricerChoice::Barrier:
        {
            unsigned short bchoice;
            double barrierAmount;
            std::cout << "Choose the type of barrier option: 1. Up-and-In, 2. Up-and-Out, 3. Down-and-In, 4. Down-and-Out\n";
            std::cin >> bchoice;
            std::cout << "Enter the barrier amount:\n";
            std::cin >> barrierAmount;
            BarrierPricer::BarrierType barrierType = static_cast<BarrierPricer::BarrierType>(bchoice);

            p = std::make_shared<BarrierPricer>(callPayoff, putPayoff, discounter, 0);
            auto barrier = std::dynamic_pointer_cast<BarrierPricer>(p);
            barrier->set_barrier_type(barrierType);
            barrier->set_barrier_amount(barrierAmount);
            break;
        }

    default:
        throw std::invalid_argument("Invalid option type. Make sure you enter a valid number.");
    }

    m_path = [p](const std::vector<double>& path)
        {
            p->process_path(path);
        };
    m_finish = [p](double duration)
        {
            p->post_process(duration);
        };
    
    return p;
}

//--------------Default Builder-----------------

// Default builder with Euler FDM, MersenneTwister RNG, and European option
template <typename SDE>
MCDefaultBuilder<SDE>::MCDefaultBuilder(const std::shared_ptr<OptionData>& optionData)
    : m_data{ optionData }, m_path{ nullptr }, m_finish{ nullptr }
{}

template <typename SDE>
MCDefaultBuilder<SDE>::PartsTuple MCDefaultBuilder<SDE>::parts()
{
    SDEBase<SDE> sde = std::move(get_SDE());
    FDMPointer fdm = std::move(get_FDM(sde));
    RNGPointer rng = std::move(get_RNG());
    PricerPointer pricer = std::move(get_pricer());

    return std::make_tuple(std::move(sde), std::move(fdm), std::move(rng));
}

template <typename SDE>
MCDefaultBuilder<SDE>::OptionPath MCDefaultBuilder<SDE>::get_path() const
{
    return m_path;
}

template <typename SDE>
MCDefaultBuilder<SDE>::Finish MCDefaultBuilder<SDE>::get_finish() const
{
    Interface::instance()->m_data = m_data;
    return m_finish;
}

template <typename SDE>
SDEBase<SDE> MCDefaultBuilder<SDE>::get_SDE() const
{
    if constexpr (std::is_same_v<SDE, GBM>)
    { // GBM
        return SDEBase<SDE>(GBM(m_data));
    }
    else
    {
        return SDEBase<SDE>(CEV(m_data));
    }
}

template <typename SDE>
MCDefaultBuilder<SDE>::FDMPointer MCDefaultBuilder<SDE>::get_FDM(const SDEBase<SDE>& sde) const
{
    return std::make_unique<EulerFDM<SDE>>(sde, 500);
}

template <typename SDE>
MCDefaultBuilder<SDE>::RNGPointer MCDefaultBuilder<SDE>::get_RNG() const
{
    return std::make_unique<MersenneTwister>();
}

template <typename SDE>
MCDefaultBuilder<SDE>::PricerPointer MCDefaultBuilder<SDE>::get_pricer()
{
    auto callPayoff = [this](double a) { return std::max(a - m_data->K, 0.0); };
    auto putPayoff = [this](double a) { return std::max(m_data->K - a, 0.0); };
    auto discounter = [this]() { return std::exp(-m_data->r * m_data->T); };

    PricerPointer p = std::make_shared<EuropeanPricer>(callPayoff, putPayoff, discounter, 0);
    m_path = [p](const std::vector<double>& path)
        {
            p->process_path(path);
        };
    m_finish = [p](double duration)
        {
            p->post_process(duration);
        };
    return p;
}
