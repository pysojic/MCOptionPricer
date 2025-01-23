// FDMAbstract.cpp
// 
// Implementation of FDMAbstract.hpp
// 
// Pierre-Yves Sojic
//


#ifndef FDMABSTRACT_CPP
#define FDMABSTRACT_CPP

#include "FDMAbstract.hpp"
#include "SDEBase.hpp"

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

#endif // !FDMABSTRACT_CPP