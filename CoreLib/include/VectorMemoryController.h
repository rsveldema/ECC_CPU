#pragma once

#include <string>
#include <iostream>

#include "RawMemoryBus.h"
#include "VecMemoryBus.h"

namespace Simulator
{
	class VectorMemoryController : public SimComponent
	{
	public:
		VecMemoryBus& toCPU;
		RawMemoryBus& toMemory;

		VectorMemoryController(SimComponentRegistry& registry,
			const std::string& name,
			VecMemoryBus& toCPU,
			RawMemoryBus& toMemory) : SimComponent(registry, name), toCPU(toCPU), toMemory(toMemory)
		{
			std::cerr << "allocated: " << name << std::endl;
		}

		coro::ReturnObject run() override;
	};
}