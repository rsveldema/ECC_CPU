#pragma once


#include <vector>
#include <queue>
#include <memory>

#include "SimComponent.h"

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
				i++;
			}
		}
	};



	class FetchToDecodeBus
	{
	public:
		class Packet
		{

		};
	};

	class FetchStage : public SimComponent
	{
	private:
		FetchToDecodeBus& decode_bus;

	public:
		FetchStage(SimComponentRegistry& registry, FetchToDecodeBus& decode_bus) : SimComponent(registry, "fetch"), decode_bus(decode_bus)
		{}

		coro::ReturnObject run() override
		{
			while (1)
			{
				co_await *this;
			}
		}
	};

	class DecodeStage : public SimComponent
	{
	private:
		FetchToDecodeBus& fetch_bus;

	public:
		DecodeStage(SimComponentRegistry& registry, FetchToDecodeBus& fetch_bus) : SimComponent(registry, "decode"), fetch_bus(fetch_bus)
		{}

		coro::ReturnObject run() override
		{
			while (1)
			{
				co_await *this;
			}
		}

	};

	class ExecuteStage : public SimComponent
	{
	public:
		ExecuteStage(SimComponentRegistry& registry) : SimComponent(registry, "execute")
		{}

		coro::ReturnObject run() override
		{
			while (1)
			{
				co_await *this;
			}
		}

	};

	class StoreStage : public SimComponent
	{
	public:
		StoreStage(SimComponentRegistry& registry) : SimComponent(registry, "storer")
		{}

		coro::ReturnObject run() override
		{
			while (1)
			{
				co_await *this;
			}
		}
	};

	class Core
	{
	private:

		FetchToDecodeBus fetch_decode_bus;
		FetchStage fetch;
		DecodeStage decode;
		ExecuteStage execute;
		StoreStage store;

	public:
		Core(SimComponentRegistry& registry)
			: fetch(registry, fetch_decode_bus),
			decode(registry, fetch_decode_bus),
			execute(registry),
			store(registry)
		{
		}
	};

	class MemoryBus
	{
	public:
		class Packet
		{
			unsigned address;
			char payload[16];
		};

		std::queue<Packet> queue;

		std::unique_ptr<Packet> acceptIncoming();
	};

	class Cache : public SimComponent
	{
	public:
		MemoryBus& toCPU;
		MemoryBus& toMemory;

		Cache(SimComponentRegistry& registry,
			const std::string& name,
			MemoryBus& toCPU,
			MemoryBus& toMemory) : SimComponent(registry, name), toCPU(toCPU), toMemory(toMemory) {}

		coro::ReturnObject run() override
		{
			std::cerr << "booted: " << name << std::endl;
			while (1)
			{
				co_await *this;
			}
		}
	};

	class DRAM : public SimComponent
	{
	public:
		MemoryBus& toCPU;
		std::vector<byte> storage;

		DRAM(SimComponentRegistry& registry,
			MemoryBus& toCPU) : SimComponent(registry, "DRAM"), toCPU(toCPU) {}

		coro::ReturnObject run() override
		{
			while (1)
			{
				co_await *this;
			}
		}
	};

	class Processor
	{
	public:
		std::vector<std::unique_ptr<Core>> cores;

		Processor(SimComponentRegistry& registry, int num_cores)
		{
			for (int i = 0; i < num_cores; i++)
			{
				cores.emplace_back(std::make_unique<Core>(registry));
			}
		}
	};

	class RegisterFile
	{
	};

	class Machine
	{
	public:
		MemoryBus L3_DRAM;
		MemoryBus L2_L3;
		MemoryBus L1_L2;
		MemoryBus core_L1;

		Cache L1;
		Cache L2;
		Cache L3;

		DRAM dram;

		Processor processor;

		Machine(SimComponentRegistry& registry, int num_cores)
			: L1(registry, "L1", core_L1, L1_L2),
			L2(registry, "L2", L1_L2, L2_L3),
			L3(registry, "L3", L2_L3, L3_DRAM),
			dram(registry, L3_DRAM),
			processor(registry, num_cores)
		{
		}
	};
}