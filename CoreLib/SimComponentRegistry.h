#pragma once

#include <vector>

namespace Simulator
{
	class SimComponent;
	class CoreClusterGrid;

	class SimComponentRegistry
	{
	public:
		std::vector<SimComponent*> components;

	public:
		void registerComponent(SimComponent* component)
		{
			components.push_back(component);
		}

		void run(CoreClusterGrid& machine);
	};
}
