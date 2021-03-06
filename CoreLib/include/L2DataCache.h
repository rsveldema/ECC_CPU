#pragma once

#include <string>
#include <iostream>

#include "SimComponent.h"
#include "MemoryBus.h"
#include "VecMemoryBus.h"

namespace ecc
{
	class L2DataCache : public SimComponent
	{
	public:
		MemoryBus& toCPU;
		MemoryBus& toMemory;

		L2DataCache(SimComponentRegistry& registry,
			const std::string& name,
			MemoryBus& toCPU,
			MemoryBus& toMemory) : SimComponent(registry, name), toCPU(toCPU), toMemory(toMemory)
		{
			std::cerr << "allocated: " << name << std::endl;
		}

		ecc::ReturnObject run() override;
	};
}