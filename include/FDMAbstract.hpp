// FDMAbstract.hpp
// 
// Abstract base class for FD methods.
// 
// Pierre-Yves Sojic
//

#pragma once

#include <concepts>

#include "SDEBase.hpp"

template <typename SDE>
class FDMAbstract
{
public:
    FDMAbstract(const SDEBase<SDE>& sde, std::size_t NT);
    virtual ~FDMAbstract() = default;

	virtual double advance(double  xn, double  tn, double  dt, double  WienerIncrement, double  WienerIncrement2) const = 0;

    std::size_t get_NT() const;
    std::vector<double> get_mesh() const;
    double get_meshSize() const;

protected:
    SDEBase<SDE> m_SDE;            // SDE on which FDM is performed
    std::size_t m_NT;	           // Number of subdivisions
    std::vector<double> m_mesh;    // The mesh array
    double m_meshSize;			   // Mesh size
};

#ifndef FDMABSTRACT_CPP
#include "FDMAbstract.cpp"
#endif // !FDMABSTRACT_CPP
