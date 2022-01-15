#pragma once

#include "SimComponent.h"
#include "MemoryBus.h"
#include "FetchDecodeBus.h"

namespace Simulator
{
	class FetchStage : public SimComponent
	{
	private:
		FetchToDecodeBus& decode_bus;
		MemoryBus& memory_bus;
		uint64_t fetch_PC = 0;
		MemoryBus::BusID memory_bus_id;

	public:
		FetchStage(SimComponentRegistry& registry, FetchToDecodeBus& decode_bus, MemoryBus& memory_bus, MemoryBus::BusID id)
			: SimComponent(registry, "fetch"), decode_bus(decode_bus), memory_bus(memory_bus), memory_bus_id(id)
		{}

		coro::ReturnObject run() override;
	};

}