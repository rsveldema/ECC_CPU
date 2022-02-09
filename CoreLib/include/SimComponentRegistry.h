#pragma once

#include <vector>
#include "Stats.h"

namespace ecc
{
	class SimComponent;
	class CoreClusterGrid;

	class SimComponentRegistry
	{
	public:
		std::vector<SimComponent*> components;

	public:
		SimComponentRegistry()
		{}


		void registerComponent(SimComponent* component)
		{
			components.push_back(component);
		}

		void run(CoreClusterGrid& machine);
	};
}
