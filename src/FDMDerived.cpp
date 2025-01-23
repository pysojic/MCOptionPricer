// FDMDerived.cpp
// 
// Implementation of FDM Derived.hpp
// 
// Pierre-Yves Sojic
//

#ifndef FDMDERIVED_CPP
#define FDMDERIVED_CPP

#include <cmath>
#include <vector>

#include "FDMAbstract.hpp"
#include "FDMDerived.hpp"

//--------------Euler-----------------

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

#endif // !FDMDERIVED_CPP