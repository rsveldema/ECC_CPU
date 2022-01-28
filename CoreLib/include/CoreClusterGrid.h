#pragma once

#include "Stats.h"
#include "MachineConfig.h"

#include "L2DataCache.h"
#include "L2InsnCache.h"

#include "L3Cache.h"

#include "Multiplexer.h"

namespace Simulator
{
	class CoreClusterGrid
	{
	public:
		RawMemoryBus L3_DRAM;
		RawMemoryBus L2d_multiplexer_bus;
		RawMemoryBus L2i_multiplexer_bus;
		RawMemoryBus L2_L3;
		RawMemoryBus core_to_L2d;
		InsnCacheMemoryBus core_to_L2i;

		L2DataCache L2d;
		L2InsnCache L2i;

		Multiplexer<RawMemoryBus> l2di_multiplexer;

		L3Cache L3;

		RAM dram;

		CoreCluster coreCluster;

		CoreClusterGrid(SimComponentRegistry& registry, const MachineConfig& config, GlobalStats& stats)
			:
			L2d(registry, "L2", core_to_L2d, L2d_multiplexer_bus),
			L2i(registry, "L2", core_to_L2i, L2i_multiplexer_bus),
			l2di_multiplexer(registry, L2_L3),
			L3(registry, "L3", L2_L3, L3_DRAM),
			dram(registry, L3_DRAM, config.grid_mem_config),
			coreCluster(registry, core_to_L2d, core_to_L2i, stats, config)
		{
			l2di_multiplexer.addInput(&L2d_multiplexer_bus, [](const RawMemoryBus::Packet& p) {
				return p.source.within_core_id != MachineInfo::CoreComponentID::FETCH;
				});
			l2di_multiplexer.addInput(&L2i_multiplexer_bus, [](const RawMemoryBus::Packet& p) {
				return p.source.within_core_id == MachineInfo::CoreComponentID::FETCH;
				});
		}

		bool hasHalted()
		{
			return coreCluster.hasHalted();
		}
	};
}