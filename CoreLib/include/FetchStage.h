#pragma once

#include "SimComponent.h"
#include "MemoryBus.h"
#include "FetchToDecodeBus.h"
#include "Stats.h"
#include "StoreToFetchBus.h"
#include "Logger.h"

namespace ecc
{
	template<CoreID core_id>
	class FetchStage : public SimComponent
	{
	private:
		FetchToDecodeBus &_decode_bus;
		MemoryBus &_memory_bus;
		StoreToFetchBus &_store_bus;

	public:
		FetchStage(SimComponentRegistry &registry, 
					FetchToDecodeBus &_decode_bus, 
				   	MemoryBus &_memory_bus, 
				   	StoreToFetchBus &_store_bus)
			: SimComponent(registry, "fetch"),
			  _decode_bus(_decode_bus),
			  _memory_bus(_memory_bus),
			  _store_bus(_store_bus)
		{
		}

		ReturnObject run() override
		{
			return run(_decode_bus, _store_bus, _memory_bus);
		}

		ReturnObject run(FetchToDecodeBus &decode_bus,
						 StoreToFetchBus &store_bus,
						 MemoryBus &memory_bus);
	};

	template class FetchStage<CORE_ID_Core0>;
	template class FetchStage<CORE_ID_Core1>;
	template class FetchStage<CORE_ID_Core2>;
	template class FetchStage<CORE_ID_Core3>;
	template class FetchStage<CORE_ID_Core4>;
	template class FetchStage<CORE_ID_Core5>;
	template class FetchStage<CORE_ID_Core6>;
	template class FetchStage<CORE_ID_Core7>;
}