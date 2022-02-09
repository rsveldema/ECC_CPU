#pragma once

#include "Stats.h"
#include "MachineConfig.h"

#include "L2DataCache.h"
#include "L2InsnCache.h"

#include "L3Cache.h"

#include "Multiplexer.h"
#include "RAM.h"
#include "CoreCluster.h"

namespace ecc
{
	class CoreClusterGrid
	{
	public:
		MemoryBus L3_DRAM = create_memory_bus();
		MemoryBus L2d_multiplexer_bus = create_memory_bus();
		MemoryBus L2i_multiplexer_bus = create_memory_bus();
		MemoryBus L2_L3 = create_memory_bus();
		MemoryBus core_to_L2d = create_memory_bus();
		MemoryBus core_to_L2i = create_memory_bus();

		L2DataCache L2d;
		L2InsnCache L2i;

		Multiplexer<MemoryBus> l2di_multiplexer;

		L3Cache L3;

		RAM dram;

		CoreCluster coreCluster;

		CoreClusterGrid(SimComponentRegistry &registry, const MachineConfig &config, GlobalStats &stats)
			: L2d(registry, "L2", core_to_L2d, L2d_multiplexer_bus),
			  L2i(registry, "L2", core_to_L2i, L2i_multiplexer_bus),
			  l2di_multiplexer(registry, L2_L3),
			  L3(registry, "L3", L2_L3, L3_DRAM),
			  dram(registry, L3_DRAM, config.grid_mem_config),
			  coreCluster(registry, core_to_L2d, core_to_L2i, stats, config)
		{
			l2di_multiplexer.addInput(&L2d_multiplexer_bus, [](const BusPacket &p)
									  { return p.source.within_core_id != CoreComponentID::FETCH; });
			l2di_multiplexer.addInput(&L2i_multiplexer_bus, [](const BusPacket &p)
									  { return p.source.within_core_id == CoreComponentID::FETCH; });
		}

		bool hasHalted()
		{
			return coreCluster.hasHalted();
		}
	};
}