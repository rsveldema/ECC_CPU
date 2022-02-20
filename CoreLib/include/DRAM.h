#pragma once

#include <vector>
#include "SimComponent.h"
#include "MemoryBus.h"

namespace ecc
{
	class DRAM : public SimComponent
	{
	public:
		MemoryBus& toCPU;

		DRAM(SimComponentRegistry& registry,
			MemoryBus& _toCPU)

			: SimComponent(registry, "RAM"),
			toCPU(_toCPU)
		{
		}

		ReturnObject run() override
		{
			return run(toCPU);
		}

		ecc::ReturnObject run(MemoryBus& toCPU);	
	};

	void INITIAL_write_to_global_memory(uint64_t address, uint8_t data);
}