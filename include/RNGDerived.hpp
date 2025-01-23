// RNGDerived.hpp
// 
// Derived classes for Random Numbers Generators
// Currently supports Mersenne Twister, Polar Marsaglia and Box-Muller
//
// Pierre-Yves Sojic
//

#pragma once

#include <random>

#include "RNGAbstract.hpp"

class MersenneTwister : public RNGAbstract
{
public:
    double generate_rn() const override;

private:
    // We use a static thread local random engine to enforce 1 engine per thread
    // thus avoiding any data race issues
    static thread_local std::mt19937_64 m_randomEngine;
};

class PolarMarsagliaNet : public RNGAbstract
{
public:
    double generate_rn() const override;

private:
    static thread_local std::default_random_engine m_randomEngine;
};

class BoxMuller : public RNGAbstract
{
public:
    double generate_rn() const override;

private:
    static thread_local std::default_random_engine m_randomEngine;
};