#pragma once

#include "SimComponent.h"
#include "ExecuteToStoreBus.h"
#include "VecMemoryBus.h"
#include "StoreToFetchBus.h"
#include "DivergenceQueue.h"
#include "MachineInfo.h"

namespace ecc
{

	class StoreStage : public SimComponent
	{
	private:
		VecMemoryBus& memory_bus;

		RegisterFile& regs;
		ExecuteToStoreBus& execute_bus;
		BusID memory_bus_id;
		StoreToFetchBus& fetch_bus;
		Logger& logger;
		GlobalStats& stats;
		DivergenceQueue& divergence_queue;

	public:
		StoreStage(SimComponentRegistry& registry,
			ExecuteToStoreBus& _execute_bus,
			VecMemoryBus& _memory_bus,
			RegisterFile& _regs,
			BusID bus_id,
			StoreToFetchBus& _fetch_bus,
			Logger& _logger,
			GlobalStats& _stats,
			DivergenceQueue& _divergence_queue)

			: SimComponent(registry, "storer"),
			execute_bus(_execute_bus),
			memory_bus(_memory_bus),
			regs(_regs),
			memory_bus_id(bus_id),
			fetch_bus(_fetch_bus),
			logger(_logger),
			stats(_stats),
			divergence_queue(_divergence_queue)
		{}

		ReturnObject run() override;
	};
}
