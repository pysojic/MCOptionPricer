// SDEConcrete.hpp
// 
// Actual implementation of the SDEs. Will be passed to SDEBase to check that the
// required conditions are satisfied.
// Currently supports Geometric Brownian Motion and Constant Elasticity of Variance models
//
// Pierre-Yves Sojic
//

#pragma once

#include <memory>
#include <random>

#include "OptionData.hpp"
#include "SDEBase.hpp"

//--------------GBM-----------------

class GBM
{ 
public:
    GBM(const std::shared_ptr<OptionData>& optionData);

    double expiry() const;
    double initial_condition() const;

    double drift(double S, double t) const;
    double diffusion(double S, double t) const;
    double drift_corrected(double S, double t, double B) const;
    double diffusion_derivative(double S) const;

private:
    std::shared_ptr<OptionData> m_data; // double data for the option
};

//--------------CEV-----------------

class CEV
{ 
public:
    CEV(const std::shared_ptr<OptionData>& optionData);

    double expiry() const;
    double initial_condition() const;

    double drift(double S, double t) const;
    double diffusion(double S, double t) const;
    double drift_corrected(double S, double t, double B) const;
    double diffusion_derivative(double S) const;

private:
    std::shared_ptr<OptionData> m_data; 
};