#pragma once

#include "Stats.h"
#include "MachineConfig.h"

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

		RAM dram;

		CoreCluster coreCluster;

		CoreClusterGrid(SimComponentRegistry& registry, const MachineConfig& config, GlobalStats& stats)
			:
			L2(registry, "L2", core_to_L2, L2_L3),
			L3(registry, "L3", L2_L3, L3_DRAM),
			dram(registry, L3_DRAM, config.grid_mem_config),
			coreCluster(registry, core_to_L2, stats, config)
		{
		}

		bool hasHalted()
		{
			return coreCluster.hasHalted();
		}
	};
}