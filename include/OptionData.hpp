// OptionData.hpp
//
// Encapsulate option data into a struct.
//
// Pierre-Yves Sojic

#pragma once

#include <algorithm> 

struct OptionData
{ // Option data + behaviour
	OptionData()
		: K{}, T{}, r{}, vol{}, q{}, S0{}, H{}, betaCEV{}, scale{}, type{OptionType::Call}
	{}

	double K;       // Strike
	double T;       // Time-to-maturity
	double r;       // Interest rate / drift
	double vol;     // Volatility
	double q;		// Dividend rate
	double S0;      // Initial condition

	// Extra data 
	double H;		// down and out barrier
	double betaCEV;	// elasticity factor (CEV model)
	double scale;	// scale factor in CEV model

	enum class OptionType
	{
		Call,
		Put
	};

	OptionType type;

	double myPayOffFunction(double S)
	{ // Payoff function

		if (type == OptionType::Call)
		{ // Call

			return std::max(S - K, 0.0);
		}
		else
		{ // Put
		
			return std::max(K - S, 0.0);
		}
	}
};
