#pragma once

#include <vector>
#include "Stats.h"

namespace Simulator
{
	class SimComponent;
	class CoreClusterGrid;

	class SimComponentRegistry
	{
	public:
		std::vector<SimComponent*> components;
		GlobalStats& stats;

	public:
		SimComponentRegistry(GlobalStats& _stats)
			: stats(_stats)
		{}


		void registerComponent(SimComponent* component)
		{
			components.push_back(component);
		}

		void run(CoreClusterGrid& machine);
	};
}
