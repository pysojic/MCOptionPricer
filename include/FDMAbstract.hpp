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

//------------Implementations------------

template <typename SDE>
FDMAbstract<SDE>::FDMAbstract(const SDEBase<SDE>& sde, std::size_t NT)
	: m_SDE(sde), m_NT{ NT }, m_mesh(m_NT + 1, 0.0), m_meshSize{ m_SDE.expiry() / static_cast<double>(m_NT) }
{
	for (std::size_t i = 1; i < m_mesh.size(); ++i)
	{
		m_mesh[i] = static_cast<double>(m_mesh[i - 1] + m_meshSize);
	}
}

template <typename SDE>
std::size_t FDMAbstract<SDE>::get_NT() const
{
	return m_NT;
}

template <typename SDE>
std::vector<double> FDMAbstract<SDE>::get_mesh() const
{
	return m_mesh;
}

template <typename SDE>
double FDMAbstract<SDE>::get_meshSize() const
{
	return m_meshSize;
}
