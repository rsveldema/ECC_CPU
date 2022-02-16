#pragma once

#include <vector>
#include "SimComponent.h"
#include "MemoryBus.h"
#include "RAM_Config.h"

namespace ecc
{
	class RAM : public SimComponent
	{
	public:
		MemoryBus& toCPU;
		RAM_Config config;


		RAM(SimComponentRegistry& registry,
			MemoryBus& _toCPU,
			const RAM_Config& _config)

			: SimComponent(registry, "RAM"),
			toCPU(_toCPU),
			config(_config)
		{
		}

		ecc::ReturnObject run() override;	
	};

	void write_to_global_memory(uint64_t address, uint8_t data);
}