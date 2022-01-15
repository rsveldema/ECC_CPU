#include "SimComponent.h"

namespace Simulator
{
	const int NUM_SIM_STEPS = 4;

	void SimComponentRegistry::run()
	{
		for (int i = 0; i < NUM_SIM_STEPS; i++)
		{
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