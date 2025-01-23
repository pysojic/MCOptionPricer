// SDEBase.hpp
// 
// Serves as a template class for SDEs.
// Uses concepts to enforce some conditions that the passe SDE object must satisfy
//
// Pierre-Yves Sojic
//

#pragma once

#include "OptionData.hpp"

// Interface contract specification

template<typename SDE>
concept IExpiry = requires (SDE c)
{
    c.expiry();
};

template<typename SDE>
concept IInitialCondition = requires (SDE c)
{
    c.initial_condition();
};

template<typename SDE>
concept IDiffusion = requires (SDE c, double S, double t)
{
    c.diffusion(S, t);
};

template<typename SDE>
concept IDrift = requires (SDE c, double S, double t)
{
    c.drift(S, t);
};

template<typename SDE>
concept IJump = requires (SDE c, double S, double t)
{
    c.jump(S, t);
};

template<typename SDE>
concept IConvection = requires (SDE c, double S, double t)
{
    c.convection(S, t);
}; 

template<typename SDE>
concept IReaction = requires (SDE c, double S, double t)
{
    c.reaction(S, t);
};

template<typename SDE>
    requires IExpiry<SDE>
class SDEBase
{ 
// Really a Bridge pattern
public:
    SDEBase(const SDE& sde);

    double expiry() const; // Expiry date of the option
    double initial_condition() const requires IInitialCondition<SDE>; // Initial condition of the SDE

    double diffusion(double S, double t) const requires IDiffusion<SDE>; // Diffusion term
    double drift(double S, double t) const requires IDrift<SDE>; // Drift term
    double jump(double S, double t) const requires IJump<SDE>; // Jump term
    double convection(double S, double t) const requires IConvection<SDE>; // Convection term
    double reaction(double S, double t) const requires IReaction<SDE>; // Reaction term

    double drift_corrected(double S, double t, double B) const requires IDrift<SDE>; 
    double diffusion_derivative(double S) const requires IDiffusion<SDE>;

private:
    SDE m_SDE;
};

#ifndef SDEBASE_CPP
#include "SDEBase.cpp"
#endif // !SDEBASE_CPP
