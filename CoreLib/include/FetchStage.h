#pragma once

#include "SimComponent.h"
#include "MemoryBus.h"
#include "FetchDecodeBus.h"
#include "Stats.h"

namespace Simulator
{
	class FetchStage : public SimComponent
	{
	private:
		FetchToDecodeBus& decode_bus;
		MemoryBus& memory_bus;
		uint64_t fetch_PC = 0;
		MemoryBus::BusID memory_bus_id;
		StoreToFetchBus& store_bus;
		GlobalStats& stats;

	public:
		FetchStage(SimComponentRegistry& registry, FetchToDecodeBus& _decode_bus, MemoryBus& _memory_bus, MemoryBus::BusID id,
			StoreToFetchBus& _store_bus, GlobalStats& _stats)
			: SimComponent(registry, "fetch"), decode_bus(_decode_bus),
			memory_bus(_memory_bus), memory_bus_id(id),
			store_bus(_store_bus),
			stats(_stats)
		{}

		coro::ReturnObject run() override;
	};

}