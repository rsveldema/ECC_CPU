#pragma once

#include <climits>
#include <vector>
#include <queue>
#include <memory>

#include "SimComponent.h"
#include "MachineInfo.h"

namespace Simulator
{
	class Multiplexer : public SimComponent
	{
	public:
		Multiplexer(SimComponentRegistry& registry) : SimComponent(registry, "multiplexer")
		{
		}

		coro::ReturnObject run() override
		{
			while (1)
			{
				co_await *this;
			}
		}
	};

}

#include "FetchDecodeBus.h"
#include "DecodeStage.h"
#include "ExecuteStage.h"
#include "StoreStage.h"

#include "FetchStage.h"


#include "Cache.h"
#include "DecodeToExecuteBus.h"
#include "ExecuteToStoreBus.h"

namespace Simulator
{
	class Core
	{
	private:
		RegisterFile regs;
		StoreToFetchBus store_fetch_bus;
		FetchToDecodeBus fetch_decode_bus;
		FetchStage fetch;
		DecodeToExecuteBus decode_execute_bus;
		DecodeStage decode;
		ExecuteToStoreBus execute_store_bus;
		ExecuteStage execute;
		StoreStage store;

		Cache L1;
		MemoryBus core_L1;
	public:
		Core(SimComponentRegistry& registry, MemoryBus& memory_bus, MachineInfo::CoreID core_id)
			: regs{},
			fetch(registry, fetch_decode_bus, core_L1, MemoryBus::createBusID(core_id, MachineInfo::CoreComponentID::FETCH), store_fetch_bus),
			decode(registry, fetch_decode_bus, decode_execute_bus, regs),
			execute(registry, decode_execute_bus, execute_store_bus),
			store(registry, execute_store_bus, memory_bus, regs, MemoryBus::createBusID(core_id, MachineInfo::CoreComponentID::STORE), store_fetch_bus),
			L1(registry, "L1", core_L1, memory_bus)
		{
		}

		bool hasHalted()
		{
			return regs.hasHalted();
		}
	};
}

#include "DRAM.h"

namespace Simulator
{
	class Processor
	{
	public:
		std::vector<std::unique_ptr<Core>> cores;

		Processor(SimComponentRegistry& registry, unsigned num_cores, MemoryBus& memory_bus)
		{
			for (unsigned i = 0; i < num_cores; i++)
			{
				cores.emplace_back(std::make_unique<Core>(registry, memory_bus, static_cast<MachineInfo::CoreID>(i)));
			}
		}

		bool hasHalted() const
		{
			for (unsigned i = 0; i < cores.size(); i++)
			{
				if (cores[i]->hasHalted())
				{
					return true;
				}
			}
			return false;
		}
	};

	class Machine
	{
	public:
		MemoryBus L3_DRAM;
		MemoryBus L2_L3;
		MemoryBus core_to_L2;

		Cache L2;
		Cache L3;

		DRAM dram;

		Processor processor;

		Machine(SimComponentRegistry& registry, int num_cores)
			:
			L2(registry, "L2", core_to_L2, L2_L3),
			L3(registry, "L3", L2_L3, L3_DRAM),
			dram(registry, L3_DRAM),
			processor(registry, num_cores, core_to_L2)
		{
		}

		bool hasHalted()
		{
			return processor.hasHalted();
		}
	};
}