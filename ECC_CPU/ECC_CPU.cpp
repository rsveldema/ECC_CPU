// ECC_CPU.cpp : Defines the entry point for the application.
//

#include "ECC_CPU.h"

using namespace Simulator;

int main()
{
	std::cerr << "starting simulation" << std::endl;

	SimComponentRegistry registry;
	Machine machine(registry, 1);

	registry.run();

	std::cerr << "finished simulation" << std::endl;

	return 0;
}
