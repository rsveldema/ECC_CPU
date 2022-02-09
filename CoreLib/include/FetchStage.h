#pragma once

#include "SimComponent.h"
#include "MemoryBus.h"
#include "FetchToDecodeBus.h"
#include "Stats.h"
#include "StoreToFetchBus.h"
#include "Logger.h"

namespace ecc
{
	class FetchStage : public SimComponent
	{
	private:
		FetchToDecodeBus &_decode_bus;
		MemoryBus &_memory_bus;
		BusID memory_bus_id;
		StoreToFetchBus &_store_bus;

	public:
		FetchStage(SimComponentRegistry &registry, FetchToDecodeBus &_decode_bus, 
				   MemoryBus &_memory_bus, 
				   BusID id,
				   StoreToFetchBus &_store_bus)
			: SimComponent(registry, "fetch"),
			  _decode_bus(_decode_bus),
			  _memory_bus(_memory_bus), memory_bus_id(id),
			  _store_bus(_store_bus)
		{
		}

		ReturnObject run() override
		{
			return run(_decode_bus, _store_bus, _memory_bus, memory_bus_id);
		}

		ReturnObject run(FetchToDecodeBus &decode_bus,
						 StoreToFetchBus &store_bus,
						 MemoryBus &memory_bus,
						 const BusID memory_bus_id);
	};

}