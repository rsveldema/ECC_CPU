#pragma once

namespace Simulator
{
	class Cache : public SimComponent
	{
	public:
		MemoryBus& toCPU;
		MemoryBus& toMemory;

		Cache(SimComponentRegistry& registry,
			const std::string& name,
			MemoryBus& toCPU,
			MemoryBus& toMemory) : SimComponent(registry, name), toCPU(toCPU), toMemory(toMemory) 
		{
			std::cerr << "allocated: "  << name << std::endl;
		}

		coro::ReturnObject run() override;
	};
}