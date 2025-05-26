// Final project - Monte Carlo Simulator
//
// Entry point of the program.
//
// Pierre-Yves Sojic

// Run in release for better perfs

#include <iostream>

#include "MCBuilder.hpp"
#include "MCMediator.hpp"

int main()
{
	try
	{
		// Define your option parameters
		std::shared_ptr<OptionData> od = std::make_shared<OptionData>();
		od->S0 = 100;
		od->K = 100;
		od->T = 30.0;
		od->vol = 0.3;
		od->r = 0.08;
		od->q = 0.0;

		std::cout << "MODULABLE BUILDER: \n\n";
		// Put the SDE type as template parameter
		MCBuilder<GBM> mbuilder(od);
		auto mparts = mbuilder.parts();
		auto mpath = mbuilder.get_path();
		auto mfinish = mbuilder.get_finish();
		MCMediator mediator(mparts, mpath, mfinish, 1'000'000);
		mediator.start();
	}
	catch (const std::exception& e)
	{
		std::cout << "ERROR: " << e.what() << std::endl;
	}

	return 0;
}