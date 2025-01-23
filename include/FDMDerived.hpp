// FDMDerived.hpp
// 
// Actual FDM. 
// Currently supports Euler FDM and Exact FDM.
// 
// Pierre-Yves Sojic
//

#pragma once

#include <vector>

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

#ifndef FDMDERIVED_CPP
#include "FDMDerived.cpp"
#endif // !FDMDERIVED_CPP
