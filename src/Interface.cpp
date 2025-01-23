// Interface.cpp
// 
// Implementation of the interface
// 
// Pierre-Yves Sojic
//

#pragma once

#include <iostream>

#include "Interface.hpp"

void Interface::display_european(double callprice, double putprice, std::size_t nSim, double duration) const
{
	std::cout << "\nOption parameters: S0 = " << m_data->S0 << ", K = " << m_data->K
		<< ", vol = " << m_data->vol << ", T = " << m_data->T
		<< ", r = " << m_data->r << ", q = " << m_data->q << std::endl;
	std::cout << "Number of MC simulations = " << nSim << std::endl;

	std::cout << "\nCall Price = " << callprice << ", Put Price = " << putprice << std::endl;

	std::cout << "\nTime elapsed: " << duration << "s" << std::endl;
}

void Interface::display_asian(double callprice, double putprice, double geomcallprice, double geomputprice, std::size_t nSim, double duration) const
{
	std::cout << "\nOption parameters: S0 = " << m_data->S0 << ", K = " << m_data->K
		<< ", vol = " << m_data->vol << ", T = " << m_data->T
		<< ", r = " << m_data->r << ", q = " << m_data->q << std::endl;
	std::cout << "Number of MC simulations = " << nSim << std::endl;

	std::cout << "\nCall Price = " << callprice << ", Put Price = " << putprice << std::endl;
	std::cout << "Call Price (Geometric Average) = " << geomcallprice << ", Put Price (Geometric Average) = " << geomputprice << std::endl;

	std::cout << "\nTime elapsed: " << duration << "s" << std::endl;
}

void Interface::display_barrier(double callprice, double putprice, double barrierAmount, std::size_t nSim, double duration) const
{
	std::cout << "\nOption parameters: S0 = " << m_data->S0 << ", K = " << m_data->K
		<< ", vol = " << m_data->vol << ", T = " << m_data->T
		<< ", r = " << m_data->r << ", q = " << m_data->q << std::endl;
	std::cout << "Barrier Amount: " << barrierAmount << std::endl;
	std::cout << "Number of MC simulations = " << nSim << std::endl;

	std::cout << "\nCall Price = " << callprice << ", Put Price = " << putprice << std::endl;

	std::cout << "\nTime elapsed: " << duration << "s" << std::endl;
}