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

#include "OptionData.hpp"
#include "SDEBase.hpp"
#include "FDMAbstract.hpp"
#include "PricerAbstract.hpp"
#include "RNGAbstract.hpp"

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

#ifndef MCBUILDER_CPP
#include "MCBuilder.cpp"
#endif // !MCBUILDER_CPP
