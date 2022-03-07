#pragma once


#include <string>
#include <iostream>

#include "SimComponent.h"
#include "MemoryBus.h"

namespace ecc
{
	template <CoreID core_id>
	class L1DataCache : public SimComponent
	{
	public:
		MemoryBus& toCPU;
		MemoryBus& toMemory;

		L1DataCache(SimComponentRegistry& registry,
			const std::string& name,
			MemoryBus& toCPU,
			MemoryBus& toMemory) : SimComponent(registry, name), toCPU(toCPU), toMemory(toMemory)
		{
			std::cerr << "allocated: " << name << std::endl;
		}

		ecc::ReturnObject run() override;
	};

	template class L1DataCache<CORE_ID_Core0>;
	template class L1DataCache<CORE_ID_Core1>;
	template class L1DataCache<CORE_ID_Core2>;
	template class L1DataCache<CORE_ID_Core3>;
	template class L1DataCache<CORE_ID_Core4>;
	template class L1DataCache<CORE_ID_Core5>;
	template class L1DataCache<CORE_ID_Core6>;
	template class L1DataCache<CORE_ID_Core7>;
}