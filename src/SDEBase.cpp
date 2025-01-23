// SDEBase.cpp
// 
// Serves as a template class for SDEs.
// Implementation of the hpp file.
//
// Pierre-Yves Sojic
//

#ifndef SDEBASE_CPP
#define SDEBASE_CPP

#include <random>
#include <iostream>

#include "SDEBase.hpp"

template <typename SDE>
    requires IExpiry<SDE>
SDEBase<SDE>::SDEBase(const SDE& sde)
    : m_SDE{ sde }
{}

template <typename SDE>
    requires IExpiry<SDE>
double SDEBase<SDE>::expiry() const
{
    return m_SDE.expiry();
}

template <typename SDE>
    requires IExpiry<SDE>
double SDEBase<SDE>::initial_condition() const
    requires IInitialCondition<SDE>
{
    return m_SDE.initial_condition();
}

template <typename SDE>
    requires IExpiry<SDE>
double SDEBase<SDE>::diffusion(double S, double t) const
    requires IDiffusion<SDE>
{
    return m_SDE.diffusion(S, t);
}

template <typename SDE>
    requires IExpiry<SDE>
double SDEBase<SDE>::drift(double S, double t) const
    requires IDrift<SDE>
{
    return m_SDE.drift(S, t);
}

template <typename SDE>
    requires IExpiry<SDE>
double SDEBase<SDE>::jump(double S, double t) const
    requires IJump<SDE>
{
    return m_SDE.jump(S, t);
}

template <typename SDE>
    requires IExpiry<SDE>
double SDEBase<SDE>::convection(double S, double t) const
    requires IConvection<SDE>
{
    return m_SDE.convection(S, t);
}


template <typename SDE>
    requires IExpiry<SDE>
double SDEBase<SDE>::reaction(double S, double t) const
    requires IReaction<SDE>
{
    return m_SDE.reaction(S, t);
}

template <typename SDE>
    requires IExpiry<SDE>
double SDEBase<SDE>::drift_corrected(double S, double t, double B) const
    requires IDrift<SDE>
{
    return m_SDE.drift_corrected(S, t);
}

template <typename SDE>
    requires IExpiry<SDE>
double SDEBase<SDE>::diffusion_derivative(double S) const
    requires IDiffusion<SDE>
{
    return m_SDE.diffusion_derivative();
}

#endif // !SDEBASE_CPP