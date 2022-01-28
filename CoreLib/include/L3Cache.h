#pragma once

#include <string>
#include <iostream>

namespace ecc
{
	class L3Cache : public SimComponent
	{
	public:
		RawMemoryBus& toCPU;
		RawMemoryBus& toMemory;

		L3Cache(SimComponentRegistry& registry,
			const std::string& name,
			RawMemoryBus& toCPU,
			RawMemoryBus& toMemory) : SimComponent(registry, name), toCPU(toCPU), toMemory(toMemory)
		{
			std::cerr << "allocated: " << name << std::endl;
		}

		ecc::ReturnObject run() override;
	};
}