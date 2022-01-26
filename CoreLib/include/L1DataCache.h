#pragma once


#include <string>
#include <iostream>

#include "SimComponent.h"
#include "RawMemoryBus.h"

namespace Simulator
{
	class L1DataCache : public SimComponent
	{
	public:
		RawMemoryBus& toCPU;
		RawMemoryBus& toMemory;

		L1DataCache(SimComponentRegistry& registry,
			const std::string& name,
			RawMemoryBus& toCPU,
			RawMemoryBus& toMemory) : SimComponent(registry, name), toCPU(toCPU), toMemory(toMemory)
		{
			std::cerr << "allocated: " << name << std::endl;
		}

		coro::ReturnObject run() override;
	};
}