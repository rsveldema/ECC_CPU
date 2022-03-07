#pragma once

#include <string>
#include <iostream>

#include "SimComponent.h"
#include "MemoryBus.h"

namespace ecc
{
	template <CoreID core_id>
	class L1InsnCache : public SimComponent
	{
	public:
		MemoryBus& _toCPU;
		MemoryBus& _toMemory;
 		GlobalStats& _stats;

		L1InsnCache(SimComponentRegistry& registry,
			const std::string& name,
			MemoryBus& toCPU,
			MemoryBus& toMemory,
			GlobalStats& stats) 
		: SimComponent(registry, name), 
			_toCPU(toCPU), 
			_toMemory(toMemory), 
			_stats(stats)
		{
			std::cerr << "allocated: " << name << std::endl;
		}

		ReturnObject run() override
		{
			return run(_toCPU, _toMemory, _stats);
		}

		ReturnObject run(MemoryBus& toCPU, MemoryBus& toMemory, GlobalStats& stats);		
	};

	template class L1InsnCache<CORE_ID_Core0>;
	template class L1InsnCache<CORE_ID_Core1>;
	template class L1InsnCache<CORE_ID_Core2>;
	template class L1InsnCache<CORE_ID_Core3>;
	template class L1InsnCache<CORE_ID_Core4>;
	template class L1InsnCache<CORE_ID_Core5>;
	template class L1InsnCache<CORE_ID_Core6>;
	template class L1InsnCache<CORE_ID_Core7>;
}