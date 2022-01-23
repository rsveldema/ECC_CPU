#pragma once

#include "SimComponent.h"
#include "ExecuteToStoreBus.h"
#include "MemoryBus.h"
#include "StoreToFetchBus.h"
#include "DivergenceQueue.h"

namespace Simulator
{

	class StoreStage : public SimComponent
	{
	private:
		MemoryBus& memory_bus;

		RegisterFile& regs;
		ExecuteToStoreBus& execute_bus;
		MemoryBus::BusID memory_bus_id;
		StoreToFetchBus& fetch_bus;
		Logger& logger;
		GlobalStats& stats;
		DivergenceQueue& divergence_queue;

	public:
		StoreStage(SimComponentRegistry& registry,
			ExecuteToStoreBus& _execute_bus,
			MemoryBus& _memory_bus,
			RegisterFile& _regs,
			MemoryBus::BusID bus_id,
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

		coro::ReturnObject run() override;

		void push_thread_context(MachineInfo::memory_address_t new_address, const ExecutionMask& exec_mask_new_address);
	};
}
