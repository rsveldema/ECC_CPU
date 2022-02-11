#pragma once

#include "SimComponent.h"
#include "ExecuteToStoreBus.h"
#include "VecMemoryBus.h"
#include "StoreToFetchBus.h"
#include "DivergenceQueue.h"
#include "MachineInfo.h"

namespace ecc
{
	template<CoreID core_id>
	class StoreStage : public SimComponent
	{
	private:
		VecMemoryBus& memory_bus;
		RegisterFile& regs;
		ExecuteToStoreBus& execute_bus;
		StoreToFetchBus& fetch_bus;
		DivergenceQueue& divergence_queue;

	public:
		StoreStage(SimComponentRegistry& registry,
			ExecuteToStoreBus& _execute_bus,
			VecMemoryBus& _memory_bus,
			RegisterFile& _regs,
			StoreToFetchBus& _fetch_bus,
			DivergenceQueue& _divergence_queue)

			: SimComponent(registry, "storer"),
			execute_bus(_execute_bus),
			memory_bus(_memory_bus),
			regs(_regs),
			fetch_bus(_fetch_bus),
			divergence_queue(_divergence_queue)
		{}

		ReturnObject run() override;
	};

	template class StoreStage<CORE_ID_Core0>;
	template class StoreStage<CORE_ID_Core1>;
	template class StoreStage<CORE_ID_Core2>;
	template class StoreStage<CORE_ID_Core3>;
	template class StoreStage<CORE_ID_Core4>;
	template class StoreStage<CORE_ID_Core5>;
	template class StoreStage<CORE_ID_Core6>;
	template class StoreStage<CORE_ID_Core7>;

}
