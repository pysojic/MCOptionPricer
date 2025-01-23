// Interface.hpp
// 
// Interface used to display the results of the MC simualtion
// 
// Pierre-Yves Sojic
//

#pragma once

#include "OptionData.hpp"
#include "Singleton.hpp"

class Interface : public Singleton<Interface>
{
public:
	void display_european(double callprice, double putprice, std::size_t nSim, double duration) const;
	void display_asian(double callprice, double putprice, double geomcallprice, double geomputprice, std::size_t nSim, double duration) const;
	void display_barrier(double callprice, double putprice, double barrierAmount, std::size_t nSim, double duration) const;

public:
	std::shared_ptr<OptionData> m_data;
};