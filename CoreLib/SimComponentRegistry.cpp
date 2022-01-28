#include "SimComponent.h"
#include "CoreLib.h"

namespace ecc
{
	const int NUM_SIM_STEPS = 1000000;

	void SimComponentRegistry::run(CoreClusterGrid& machine)
	{
		for (int i = 0; i < NUM_SIM_STEPS; i++)
		{
			stats.numCyclesUsed++;

			if (machine.hasHalted())
			{
				break;
			}

			if (i == 0)
			{
				for (auto* c : components)
				{
					std::cerr << "booted: " << c->name << std::endl;
					c->run();
				}
			}
			else
			{
				for (auto* c : components)
				{
					c->resume();
				}
			}
		}
	}


}