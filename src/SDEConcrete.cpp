// SDEConcrete.cpp
// 
// Implementation of SDEConcrete.hpp
// Currently supports Geometric Brownian Motion and Constant Elasticity of Variance models
//
// Pierre-Yves Sojic
//

#include <cmath>

#include "OptionData.hpp"
#include "SDEConcrete.hpp"

//--------------GBM-----------------

GBM::GBM(const std::shared_ptr<OptionData>& optionData)
	: m_data(optionData) 
{}

double GBM::expiry() const
{
	return m_data->T;
}

double GBM::initial_condition() const
{
	return m_data->S0;
}

double GBM::drift(double S, double t) const
{
	// Drift term
	return (m_data->r - m_data->q) * S; // r - q
}

double GBM::diffusion(double S, double t) const
{
	// Diffusion term
	return m_data->vol * S;
}

double GBM::drift_corrected(double S, double t, double B) const
{
	return drift(S, t) - B * diffusion(S, t) * diffusion_derivative(S);
}

double GBM::diffusion_derivative(double S) const
{
	return m_data->vol;
}

//--------------CEV-----------------

CEV::CEV(const std::shared_ptr<OptionData>& optionData) : m_data(optionData)
{}

double CEV::expiry() const
{
	return m_data->T;
}

double CEV::initial_condition() const
{
	return m_data->S0;
}

double CEV::drift(double S, double t) const
{ // Drift term
	return (m_data->r - m_data->q) * S;
}

double CEV::diffusion(double S, double t) const
{ // Diffusion term
	return m_data->vol * pow(S, m_data->betaCEV);
}

double CEV::drift_corrected(double S, double t, double B) const
{
	return drift(S, t) - B * diffusion(S, t) * diffusion_derivative(S);
}

double CEV::diffusion_derivative(double S) const
{
	if (m_data->betaCEV > 1.0)
	{
		return m_data->vol * m_data->betaCEV * pow(S, m_data->betaCEV - 1.0);
	}
	else
	{
		return m_data->vol * m_data->betaCEV / pow(S, 1.0 - m_data->betaCEV);
	}

}