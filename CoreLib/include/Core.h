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
		MemoryBus core_L1d;

		// instruction cache
		L1InsnCache L1i;
		MemoryBus core_L1i;


		MemoryBus L1i_to_L2i;
		MemoryBus L1d_to_l2d;
		//RawMemoryBus idle_core_memory_bus;
		VecMemoryBus store_to_vec_controller_bus;
		VectorMemoryController vecMemController;

	public:
		Core(SimComponentRegistry& registry, CoreID core_id)
			: regs{},
			fetch(registry, fetch_decode_bus, core_L1i, createBusID(core_id, CoreComponentID::COMPONENT_TYPE_FETCH),
				store_fetch_bus),
			decode(registry, fetch_decode_bus, decode_execute_bus, regs),
			execute(registry, decode_execute_bus, execute_store_bus, regs),
			store(registry, execute_store_bus, store_to_vec_controller_bus, 
					regs, createBusID(core_id, CoreComponentID::COMPONENT_TYPE_STORE),
				store_fetch_bus, divergence_queue),
			L1i(registry, "L1i", core_L1i, L1i_to_L2i),
			L1d(registry, "L1d", core_L1d, L1d_to_l2d),
			vecMemController(registry, "vec_mem_controller", store_to_vec_controller_bus, core_L1d)
		{
		}

		MemoryBus& getExternalDataBus() {
			return L1d_to_l2d;
		}

		MemoryBus& getExternalInsnBus() {
			return L1i_to_L2i;
		}

		bool hasHalted()
		{
			return regs.hasHalted();
		}
	};
}
