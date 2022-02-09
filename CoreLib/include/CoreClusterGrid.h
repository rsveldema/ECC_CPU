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
		MemoryBus L3_DRAM;
		MemoryBus L2d_multiplexer_bus;
		MemoryBus L2i_multiplexer_bus;
		MemoryBus L2_L3;
		MemoryBus core_to_L2d;
		MemoryBus core_to_L2i;

		L2DataCache L2d;
		L2InsnCache L2i;

		Multiplexer<MemoryBus> l2di_multiplexer;

		L3Cache L3;

		RAM dram;

		CoreCluster coreCluster;

		CoreClusterGrid(SimComponentRegistry &registry, const MachineConfig &config)
			: L2d(registry, "L2", core_to_L2d, L2d_multiplexer_bus),
			  L2i(registry, "L2", core_to_L2i, L2i_multiplexer_bus),
			  l2di_multiplexer(registry, L2_L3),
			  L3(registry, "L3", L2_L3, L3_DRAM),
			  dram(registry, L3_DRAM, config.grid_mem_config),
			  coreCluster(registry, core_to_L2d, core_to_L2i, config)
		{
			L3_DRAM.init();
			L2d_multiplexer_bus.init();
			L2i_multiplexer_bus.init();
			L2_L3.init();
			core_to_L2d.init();
			core_to_L2i.init();

			// this one takes everything that takes every pkt to the fetcher:
			l2di_multiplexer.addInput(&L2i_multiplexer_bus, [](const BusPacket &p)
									  { return p.source.within_core_id == CoreComponentID::COMPONENT_TYPE_FETCH; });

			// this takes everything else:
			l2di_multiplexer.addInput(&L2d_multiplexer_bus, [](const BusPacket &p)
									  { return p.source.within_core_id != CoreComponentID::COMPONENT_TYPE_FETCH; });
		}

		bool hasHalted()
		{
			return coreCluster.hasHalted();
		}
	};
}