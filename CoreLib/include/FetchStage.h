#pragma once

#include "SimComponent.h"
#include "InsnCacheMemoryBus.h"
#include "FetchDecodeBus.h"
#include "Stats.h"
#include "InsnCacheMemoryBus.h"
#include "StoreToFetchBus.h"
#include "Logger.h"

namespace ecc
{
	class FetchStage : public SimComponent
	{
	private:
		FetchToDecodeBus& _decode_bus;
		InsnCacheMemoryBus& _memory_bus;
		ecc::BusID memory_bus_id;
		StoreToFetchBus& _store_bus;
		GlobalStats& stats;
		Logger& logger;

	public:
		FetchStage(SimComponentRegistry& registry, FetchToDecodeBus& _decode_bus, InsnCacheMemoryBus& _memory_bus, ecc::BusID id,
			StoreToFetchBus& _store_bus, GlobalStats& _stats,
			Logger& _logger)
			: SimComponent(registry, "fetch"), 
			_decode_bus(_decode_bus),
			_memory_bus(_memory_bus), memory_bus_id(id),
			_store_bus(_store_bus),
			stats(_stats),
			logger(_logger)
		{}

		ecc::ReturnObject run() override 
		{
			return run(_decode_bus, _store_bus, _memory_bus);
		}

		ReturnObject run(FetchToDecodeBus& decode_bus,
								 StoreToFetchBus& store_bus,
								 InsnCacheMemoryBus& memory_bus);
	};

}