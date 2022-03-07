#pragma once

#include "GlobalStats.h"

#include "L2DataCache.h"
#include "L2InsnCache.h"

#include "L3Cache.h"

#include "CoreInternalMemoryMultiplexer.h"
#include "DRAM.h"
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

		CoreInternalMemoryMultiplexer l2di_multiplexer;

		L3Cache L3;

		DRAM dram;

		CoreCluster coreCluster;

		void init()
		{
			L3_DRAM.init();
			L2d_multiplexer_bus.init();
			L2i_multiplexer_bus.init();
			L2_L3.init();
			core_to_L2d.init();
			core_to_L2i.init();

			coreCluster.init();
		}

		CoreClusterGrid(SimComponentRegistry &registry)
			: L2d(registry, "L2", core_to_L2d, L2d_multiplexer_bus),
			  L2i(registry, "L2", core_to_L2i, L2i_multiplexer_bus),
			  l2di_multiplexer(registry, L2_L3, L2i_multiplexer_bus, L2d_multiplexer_bus),
			  L3(registry, "L3", L2_L3, L3_DRAM),
			  dram(registry, L3_DRAM),
			  coreCluster(registry, core_to_L2d, core_to_L2i)
		{
			L3_DRAM.init();
			L2d_multiplexer_bus.init();
			L2i_multiplexer_bus.init();
			L2_L3.init();
			core_to_L2d.init();
			core_to_L2i.init();
		}

		bool hasHalted()
		{
			return coreCluster.hasHalted();
		}
	};
}