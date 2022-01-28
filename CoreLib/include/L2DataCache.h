#pragma once

#include <string>
#include <iostream>

#include "SimComponent.h"
#include "RawMemoryBus.h"
#include "VecMemoryBus.h"

namespace ecc
{
	class L2DataCache : public SimComponent
	{
	public:
		RawMemoryBus& toCPU;
		RawMemoryBus& toMemory;

		L2DataCache(SimComponentRegistry& registry,
			const std::string& name,
			RawMemoryBus& toCPU,
			RawMemoryBus& toMemory) : SimComponent(registry, name), toCPU(toCPU), toMemory(toMemory)
		{
			std::cerr << "allocated: " << name << std::endl;
		}

		ecc::ReturnObject run() override;
	};
}