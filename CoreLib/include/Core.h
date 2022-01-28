#pragma once

#include "DivergenceQueue.h"

#include "L1InsnCache.h"
#include "L1DataCache.h"

#include "L2InsnCache.h"
#include "L2DataCache.h"


#include "VectorMemoryController.h"

namespace ecc
{
	class Core
	{
	private:
		Logger core_logger;

		DivergenceQueue divergence_queue;

		RegisterFile regs;
		StoreToFetchBus store_fetch_bus;
		FetchToDecodeBus fetch_decode_bus;
		FetchStage fetch;
		DecodeToExecuteBus decode_execute_bus;
		DecodeStage decode;
		ExecuteToStoreBus execute_store_bus;
		ExecuteStage execute;
		StoreStage store;

		// data cache
		L1DataCache L1d;
		RawMemoryBus core_L1d;

		// instruction cache
		L1InsnCache L1i;
		InsnCacheMemoryBus core_L1i;


		InsnCacheMemoryBus L1i_to_L2i;
		RawMemoryBus L1d_to_l2d;

		//RawMemoryBus idle_core_memory_bus;

		VecMemoryBus store_to_vec_controller_bus;
		VectorMemoryController vecMemController;

	public:
		Core(SimComponentRegistry& registry, ecc::CoreID core_id, GlobalStats& stats)
			: core_logger(ecc::to_string(core_id)),
			regs{},
			fetch(registry, fetch_decode_bus, core_L1i, ecc::createBusID(core_id, ecc::CoreComponentID::FETCH),
				store_fetch_bus, stats, core_logger),
			decode(registry, fetch_decode_bus, decode_execute_bus, regs, core_logger),
			execute(registry, decode_execute_bus, execute_store_bus, core_logger, regs),
			store(registry, execute_store_bus, store_to_vec_controller_bus, regs, ecc::createBusID(core_id, ecc::CoreComponentID::STORE),
				store_fetch_bus, core_logger, stats, divergence_queue),
			L1i(registry, "L1i", core_L1i, L1i_to_L2i),
			L1d(registry, "L1d", core_L1d, L1d_to_l2d),
			vecMemController(registry, "vec_mem_controller", store_to_vec_controller_bus, core_L1d)
		{
		}

		RawMemoryBus& getExternalDataBus() {
			return L1d_to_l2d;
		}

		InsnCacheMemoryBus& getExternalInsnBus() {
			return L1i_to_L2i;
		}

		bool hasHalted()
		{
			return regs.hasHalted();
		}
	};
}
