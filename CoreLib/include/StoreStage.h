#pragma once

#include "SimComponent.h"
#include "ExecuteToStoreBus.h"
#include "VecMemoryBus.h"
#include "StoreToFetchBus.h"
#include "DivergenceQueue.h"
#include "MachineInfo.h"

namespace ecc
{
	template <CoreID core_id>
	class StoreStage : public SimComponent
	{
	private:
		ExecuteToStoreBus &_execute_bus;
		VecMemoryBus &_memory_bus;
		RegisterFile &_regs;
		StoreToFetchBus &_fetch_bus;
		DivergenceQueue &_divergence_queue;

	public:
		StoreStage(SimComponentRegistry &registry,
				   ExecuteToStoreBus &_execute_bus,
				   VecMemoryBus &_memory_bus,
				   RegisterFile &_regs,
				   StoreToFetchBus &_fetch_bus,
				   DivergenceQueue &_divergence_queue)

			: SimComponent(registry, "storer"),
			  _execute_bus(_execute_bus),
			  _memory_bus(_memory_bus),
			  _regs(_regs),
			  _fetch_bus(_fetch_bus),
			  _divergence_queue(_divergence_queue)
		{
		}

		ReturnObject run() override
		{
			return run(_execute_bus, _memory_bus, _regs, _fetch_bus, _divergence_queue);
		}

		ReturnObject run(ExecuteToStoreBus &execute_bus,
						 VecMemoryBus &memory_bus,
						 RegisterFile &regs,
						 StoreToFetchBus &fetch_bus,
						 DivergenceQueue &divergence_queue);
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
