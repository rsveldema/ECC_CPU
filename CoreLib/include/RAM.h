#pragma once

#include <vector>
#include "SimComponent.h"
#include "RawMemoryBus.h"
#include "RAM_Config.h"

namespace Simulator
{
	class RAM : public SimComponent
	{
	public:
		RawMemoryBus& toCPU;
		std::vector<uint8_t> storage;
		RAM_Config config;


		RAM(SimComponentRegistry& registry,
			RawMemoryBus& _toCPU,
			const RAM_Config& _config)

			: SimComponent(registry, "RAM"),
			toCPU(_toCPU),
			config(_config)
		{
			storage.resize(config.size);
		}

		coro::ReturnObject run() override;

		void write(uint64_t address, char* data, size_t len)
		{
			std::copy(data, data + len, storage.begin() + address);
		}
	};
}