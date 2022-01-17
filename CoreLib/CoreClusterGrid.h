#pragma once

namespace Simulator
{
	class CoreClusterGrid
	{
	public:
		MemoryBus L3_DRAM;
		MemoryBus L2_L3;
		MemoryBus core_to_L2;

		Cache L2;
		Cache L3;

		DRAM dram;

		CoreCluster processor;

		CoreClusterGrid(SimComponentRegistry& registry, unsigned num_cores)
			:
			L2(registry, "L2", core_to_L2, L2_L3),
			L3(registry, "L3", L2_L3, L3_DRAM),
			dram(registry, L3_DRAM),
			processor(registry, num_cores, core_to_L2)
		{
		}

		bool hasHalted()
		{
			return processor.hasHalted();
		}
	};
}