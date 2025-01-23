// RNGDerived.cpp
// 
// Implementation of hpp file
//
// Pierre-Yves Sojic
//

#include <random>

#include "RNGDerived.hpp"

thread_local std::mt19937_64 MersenneTwister::m_randomEngine{ std::random_device{}() };

double MersenneTwister::generate_rn() const
{
    std::normal_distribution<double> normDist(0.0, 1.0);

    return normDist(m_randomEngine);
}

thread_local std::default_random_engine PolarMarsagliaNet::m_randomEngine{ std::random_device{}() };

double PolarMarsagliaNet::generate_rn() const
{
    std::uniform_real_distribution<double> unifDist(0.0, 1.0);
        
    double u, v, S;

    do
    {
        u = 2.0 * unifDist(m_randomEngine) - 1.0;
        v = 2.0 * unifDist(m_randomEngine) - 1.0;
        S = u * u + v * v;
    } while (S > 1.0 || S <= 0.0);

    double fac = std::sqrt(-2.0 * std::log(S) / S);

    return u * fac;
}

thread_local std::default_random_engine BoxMuller::m_randomEngine{ std::random_device{}() };

double BoxMuller::generate_rn() const
{
    std::uniform_real_distribution<double> unifDist(0.0, 1.0);

    double U1, U2;
    // U1 and U2 should be independent uniform random numbers
    do
    {
        U1 = unifDist(m_randomEngine);   // In interval [0,1)
        U2 = unifDist(m_randomEngine);  // In interval [0,1)
    } while (U1 <= 0.0);

    // Box-Muller method
    return std::sqrt(- 2.0 * std::log(U1)) * std::cos(2.0 * 3.1415159 * U2);
}
