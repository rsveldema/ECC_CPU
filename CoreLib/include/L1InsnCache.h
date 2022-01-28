#pragma once

#include <string>
#include <iostream>

#include "SimComponent.h"
#include "RawMemoryBus.h"
#include "InsnCacheMemoryBus.h"

namespace ecc
{
	class L1InsnCache : public SimComponent
	{
	public:
		InsnCacheMemoryBus& toCPU;
		InsnCacheMemoryBus& toMemory;

		L1InsnCache(SimComponentRegistry& registry,
			const std::string& name,
			InsnCacheMemoryBus& toCPU,
			InsnCacheMemoryBus& toMemory) : SimComponent(registry, name), toCPU(toCPU), toMemory(toMemory)
		{
			std::cerr << "allocated: " << name << std::endl;
		}

		ecc::ReturnObject run() override;
	};
}