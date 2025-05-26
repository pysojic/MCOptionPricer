// FDMDerived.hpp
// 
// Actual FDM. 
// Currently supports Euler FDM and Exact FDM.
// 
// Pierre-Yves Sojic
//

#pragma once

#include <vector>
#include <cmath>

#include "FDMAbstract.hpp"
#include "SDEBase.hpp"

//--------------Euler-----------------

template <typename SDE>
class EulerFDM : public FDMAbstract<SDE>
{
public:
    EulerFDM(const SDEBase<SDE>& sde, std::size_t m_NT);

    double advance(double xn, double tn, double dt, double normalVar, double normalVar2) const override;
};

//--------------Exact-----------------

template <typename SDE>
class ExactFDM : public FDMAbstract<SDE>
{
public:
    ExactFDM(const SDEBase<SDE>& sde, std::size_t NT, double S0, double vol, double r);

    double advance(double xn, double tn, double dt, double normalVar, double normalVar2) const override;

private:
    double m_S0;
    double m_vol;
    double m_r;
};

//------------Implementations------------

template <typename SDE>
EulerFDM<SDE>::EulerFDM(const SDEBase<SDE>& sde, std::size_t NT)
	: FDMAbstract<SDE>(sde, NT)
{}

template <typename SDE>
double EulerFDM<SDE>::advance(double xn, double  tn, double  dt, double normalVar, double normalVar2) const
{
	return xn + this->m_SDE.drift(xn, tn) * dt + this->m_SDE.diffusion(xn, tn) * std::sqrt(dt) * normalVar;
}

//--------------Exact-----------------

template <typename SDE>
ExactFDM<SDE>::ExactFDM(const SDEBase<SDE>& sde, std::size_t NT, double S0, double vol, double r)
	: FDMAbstract<SDE>(sde, NT), m_S0{ S0 }, m_vol{ vol }, m_r{ r }
{}

template <typename SDE>
double ExactFDM<SDE>::advance(double xn, double  tn, double  dt, double normalVar, double normalVar2) const
{
	// Compute exact value at tn + dt.
	double alpha = 0.5 * m_vol * m_vol;
	return m_S0 * std::exp((m_r - alpha) * (tn + dt) + m_vol * std::sqrt(tn + dt) * normalVar);
}
