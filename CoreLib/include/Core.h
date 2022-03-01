#pragma once

#include "DivergenceQueue.h"

#include "L1InsnCache.h"
#include "L1DataCache.h"

#include "L2InsnCache.h"
#include "L2DataCache.h"


#include "VectorMemoryController.h"

namespace ecc
{

	template<CoreID core_id>
	class Core
	{
	private:		
		DivergenceQueue divergence_queue;

		StoreToFetchBus store_fetch_bus;
		FetchToDecodeBus fetch_decode_bus;
		DecodeToExecuteBus decode_execute_bus;
		ExecuteToStoreBus execute_store_bus;


		RegisterFile regs;
		FetchStage<core_id> fetch;
		DecodeStage<core_id> decode;
		ExecuteStage<core_id> execute;
		StoreStage<core_id>  store;

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
		VectorMemoryController<core_id> vecMemController;

	public:

	void init()
	{
		core_L1d.init();
		core_L1i.init();
		L1i_to_L2i.init();
		L1d_to_l2d.init();
		divergence_queue.init();
		store_to_vec_controller_bus.init();

		store_fetch_bus.init();
		fetch_decode_bus.init();
		decode_execute_bus.init();
		execute_store_bus.init();
	}

	public:
		Core(SimComponentRegistry& registry)
			: regs{},
			fetch(registry, fetch_decode_bus, core_L1i, store_fetch_bus),
			decode(registry, fetch_decode_bus, decode_execute_bus, regs),
			execute(registry, decode_execute_bus, execute_store_bus, regs),
			store(registry, execute_store_bus, store_to_vec_controller_bus, 
					regs, store_fetch_bus, divergence_queue),
			L1d(registry, "L1d", core_L1d, L1d_to_l2d),
			L1i(registry, "L1i", core_L1i, L1i_to_L2i),
			vecMemController(registry, "vec_mem_controller", store_to_vec_controller_bus, core_L1d)
		{
		}

		MemoryBus& getExternalDataBus() {
			return L1d_to_l2d;
		}

		MemoryBus& getExternalInsnBus() {
			return L1i_to_L2i;
		}

		bool hasHalted() const
		{
			return regs.hasHalted();
		}
	};
}
