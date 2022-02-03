#pragma once

#include <string>
#include <iostream>

#include "SimComponent.h"
#include "MemoryBus.h"

namespace ecc
{
	class L1InsnCache : public SimComponent
	{
	public:
		MemoryBus& toCPU;
		MemoryBus& toMemory;

		L1InsnCache(SimComponentRegistry& registry,
			const std::string& name,
			MemoryBus& toCPU,
			MemoryBus& toMemory) : SimComponent(registry, name), toCPU(toCPU), toMemory(toMemory)
		{
			std::cerr << "allocated: " << name << std::endl;
		}

		ecc::ReturnObject run() override;
	};
}