#pragma once

#include "SimComponent.h"
#include "InsnCacheMemoryBus.h"
#include "FetchDecodeBus.h"
#include "Stats.h"
#include "InsnCacheMemoryBus.h"

namespace Simulator
{
	class FetchStage : public SimComponent
	{
	private:
		FetchToDecodeBus& decode_bus;
		InsnCacheMemoryBus& memory_bus;
		MachineInfo::memory_address_t fetch_PC = 0;
		MachineInfo::BusID memory_bus_id;
		StoreToFetchBus& store_bus;
		GlobalStats& stats;

	public:
		FetchStage(SimComponentRegistry& registry, FetchToDecodeBus& _decode_bus, InsnCacheMemoryBus& _memory_bus, MachineInfo::BusID id,
			StoreToFetchBus& _store_bus, GlobalStats& _stats)
			: SimComponent(registry, "fetch"), decode_bus(_decode_bus),
			memory_bus(_memory_bus), memory_bus_id(id),
			store_bus(_store_bus),
			stats(_stats)
		{}

		coro::ReturnObject run() override;
	};

}