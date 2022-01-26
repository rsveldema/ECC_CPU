#pragma once

#include "Stats.h"
#include "MachineConfig.h"

#include "L2DataCache.h"
#include "L2InsnCache.h"

#include "L3Cache.h"

namespace Simulator
{
	class CoreClusterGrid
	{
	public:
		RawMemoryBus L3_DRAM;
		RawMemoryBus L2_L3;
		RawMemoryBus core_to_L2d;
		InsnCacheMemoryBus core_to_L2i;

		L2DataCache L2d;
		L2InsnCache L2i;

		L3Cache L3;

		RAM dram;

		CoreCluster coreCluster;

		CoreClusterGrid(SimComponentRegistry& registry, const MachineConfig& config, GlobalStats& stats)
			:
			L2d(registry, "L2", core_to_L2d, L2_L3),
			L2i(registry, "L2", core_to_L2i, L2_L3),
			L3(registry, "L3", L2_L3, L3_DRAM),
			dram(registry, L3_DRAM, config.grid_mem_config),
			coreCluster(registry, core_to_L2d, core_to_L2i, stats, config)
		{
		}

		bool hasHalted()
		{
			return coreCluster.hasHalted();
		}
	};
}