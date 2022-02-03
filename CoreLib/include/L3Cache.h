#pragma once

#include <string>
#include <iostream>

namespace ecc
{
	class L3Cache : public SimComponent
	{
	public:
		MemoryBus& toCPU;
		MemoryBus& toMemory;

		L3Cache(SimComponentRegistry& registry,
			const std::string& name,
			MemoryBus& toCPU,
			MemoryBus& toMemory) : SimComponent(registry, name), toCPU(toCPU), toMemory(toMemory)
		{
			std::cerr << "allocated: " << name << std::endl;
		}

		ecc::ReturnObject run() override;
	};
}