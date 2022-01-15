#pragma once

#include <vector>
#include "SimComponent.h"
#include "MemoryBus.h"


namespace Simulator
{

	class DRAM : public SimComponent
	{
	public:
		MemoryBus& toCPU;
		std::vector<byte> storage;

		DRAM(SimComponentRegistry& registry,
			MemoryBus& toCPU) : SimComponent(registry, "DRAM"), toCPU(toCPU)
		{
			storage.resize(1024 * 1024);
		}

		coro::ReturnObject run() override;

		void write(uint64_t address, char* data, size_t len)
		{
			std::copy(data, data + len, storage.begin() + address);
		}
	};
}