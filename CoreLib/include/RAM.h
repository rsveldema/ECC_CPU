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
		std::vector<uint8_t> storage;
		RAM_Config config;


		RAM(SimComponentRegistry& registry,
			MemoryBus& _toCPU,
			const RAM_Config& _config)

			: SimComponent(registry, "RAM"),
			toCPU(_toCPU),
			config(_config)
		{
			storage.resize(config.size);
		}

		ecc::ReturnObject run() override;

		void write(uint64_t address, char* data, size_t len)
		{
			std::copy(data, data + len, storage.begin() + address);
		}
	};
}