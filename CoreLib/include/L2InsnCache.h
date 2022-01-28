#pragma once

#include <string>
#include <iostream>

#include "SimComponent.h"
#include "RawMemoryBus.h"
#include "InsnCacheMemoryBus.h"

namespace ecc
{
	class L2InsnCache : public SimComponent
	{
	public:
		InsnCacheMemoryBus& toCPU;
		RawMemoryBus& toMemory;

		L2InsnCache(SimComponentRegistry& registry,
			const std::string& name,
			InsnCacheMemoryBus& toCPU,
			RawMemoryBus& toMemory) : SimComponent(registry, name), toCPU(toCPU), toMemory(toMemory)
		{
			std::cerr << "allocated: " << name << std::endl;
		}

		ecc::ReturnObject run() override;
	};
}