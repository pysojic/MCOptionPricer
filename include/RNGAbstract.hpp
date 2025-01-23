// RNGAbstract.hpp
// 
// Base class for Random number Generators
//
// Pierre-Yves Sojic
//

#pragma once

class RNGAbstract
{
public:
    virtual double generate_rn() const = 0;
};
