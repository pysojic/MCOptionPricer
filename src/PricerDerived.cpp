// PricerDerived.cpp
// 
// Implementation of hpp file
// Currently supports European and Asian options
// 
// Pierre-Yves Sojic
//

#include <algorithm>
#include <numeric>
#include <iostream>

#include "PricerDerived.hpp"

//--------------European Option-----------------

EuropeanPricer::EuropeanPricer(const PayoffFunc& callpayoff, const PayoffFunc& putpayoff, const DiscounterFunc& discounter, std::size_t nSim)
	: PricerAbstract(callpayoff, putpayoff, discounter, nSim)
{}

void EuropeanPricer::process_path(const std::vector<double>& path)
{
	m_callSum += m_callPayoff(path.back()); // Each path simulation is added to the sum variable
	m_putSum += m_putPayoff(path.back());
	++m_NSim; // Increment the simulation counter
}

void EuropeanPricer::post_process(double duration)
{
	// End function

	m_callPrice = m_discounter() * m_callSum / m_NSim; // Take the average of the calculated prices and discounts them to time 0
	m_putPrice = m_discounter() * m_putSum / m_NSim;

	std::cout << "\n=============================\n";
	std::cout << "\nEUROPEAN OPTION: " << std::endl;

	Interface::instance()->display_european(m_callPrice, m_putPrice, m_NSim, duration); // Call the interface to display results

	std::cout << "\n=============================\n";
}

//--------------Asian Option-----------------

AsianPricer::AsianPricer(const PayoffFunc& callpayoff, const PayoffFunc& putpayoff, const DiscounterFunc& discounter, std::size_t nSim)
	: PricerAbstract(callpayoff, putpayoff, discounter, nSim), m_geom_callSum{}, m_geom_callPrice{}, m_geom_putSum{}, m_geom_putPrice{}
{}

void AsianPricer::process_path(const std::vector<double>& path)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	double avg = Average(path);
	double geom_avg = GeometricAverage(path);
    
	m_callSum += m_callPayoff(avg); 
	m_geom_callSum += m_callPayoff(geom_avg);
	m_putSum += m_putPayoff(avg);
	m_geom_putSum += m_putPayoff(geom_avg);
	++m_NSim;
}

void AsianPricer::post_process(double duration)
{
	m_callPrice = m_discounter() * m_callSum / m_NSim;
	m_geom_callPrice = m_discounter() * m_geom_callSum / m_NSim;
	m_putPrice = m_discounter() * m_putSum / m_NSim;
	m_geom_putPrice = m_discounter() * m_geom_putSum / m_NSim;

	std::cout << "\n=============================\n";
	std::cout << "\nASIAN OPTION: " << std::endl;

	Interface::instance()->display_asian(m_callPrice, m_putPrice, m_geom_callPrice, m_geom_putPrice, m_NSim, duration);

	std::cout << "\n=============================\n";
}

double AsianPricer::Average(const std::vector<double>& path)
{
	double avg = std::accumulate(path.begin(), path.end(), 0.0);

	return avg / path.size();
}

double AsianPricer::GeometricAverage(const std::vector<double>& path)
{
	double log_sum = std::accumulate(path.begin(), path.end(), 0.0, [](double sum, double value)
		{
			return sum + std::log(value);
		});

	// Calculate the geometric mean by taking the exponential of the averaged logarithm sum
	return std::exp(log_sum / path.size());
}

double AsianPricer::Max(const std::vector<double>& path)
{
	double max = *std::max_element(path.begin(), path.end());

	return max;
}

//--------------Barrier Option-----------------

BarrierPricer::BarrierPricer(const PayoffFunc& callpayoff, const PayoffFunc& putpayoff, const DiscounterFunc& discounter, std::size_t nSim)
	: PricerAbstract(callpayoff, putpayoff, discounter, nSim), m_barrierType{}, m_barrierAmount{}
{}

void BarrierPricer::process_path(const std::vector<double>& path)
{
	auto [min, max] = std::minmax_element(path.begin(), path.end());

	switch (m_barrierType)
	{
	case BarrierType::Up_and_In:
		if (*max >= m_barrierAmount)
		{
			m_callSum += m_callPayoff(path.back());
			m_putSum += m_putPayoff(path.back());
		}
		break;
	case BarrierType::Up_and_Out:
		if (*max < m_barrierAmount)
		{
			m_callSum += m_callPayoff(path.back());
			m_putSum += m_putPayoff(path.back());
		}
		break;
	case BarrierType::Down_and_In:
		if (*min <= m_barrierAmount)
		{
			m_callSum += m_callPayoff(path.back());
			m_putSum += m_putPayoff(path.back());
		}
		break;
	case BarrierType::Down_and_Out:
		if (*min > m_barrierAmount)
		{
			m_callSum += m_callPayoff(path.back());
			m_putSum += m_putPayoff(path.back());
		}
		break;
	}

	++m_NSim;
}

void BarrierPricer::post_process(double duration)
{
	// End function

	m_callPrice = m_discounter() * m_callSum / m_NSim; // Take the average of the calculated prices and discounts them to time 0
	m_putPrice = m_discounter() * m_putSum / m_NSim;

	std::cout << "\n=============================\n";
	std::cout << "\nBARRIER OPTION: " << std::endl;

	Interface::instance()->display_barrier(m_callPrice, m_putPrice, m_barrierAmount, m_NSim, duration); // Call the interface to display results

	std::cout << "\n=============================\n";
}

void BarrierPricer::set_barrier_type(BarrierType barrierType)
{
	m_barrierType = barrierType;
}

void BarrierPricer::set_barrier_amount(double barrierAmount)
{
	m_barrierAmount = barrierAmount;
}
