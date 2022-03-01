#pragma once

#include <string>
#include <iostream>

#include "MemoryBus.h"
#include "VecMemoryBus.h"

namespace ecc
{
	template<CoreID core_id>
	class VectorMemoryController : public SimComponent
	{
	public:
		VecMemoryBus& _toCPU;
		MemoryBus& _toMemory;

		VectorMemoryController(SimComponentRegistry& registry,
			const std::string& name,
			VecMemoryBus& toCPU,
			MemoryBus& toMemory) 
		: SimComponent(registry, name), 
			_toCPU(toCPU), 
			_toMemory(toMemory)
		{
			std::cerr << "allocated: " << name << std::endl;
		}

		ReturnObject run() override
		{
			return run(_toCPU, _toMemory);
		}

		ReturnObject run(VecMemoryBus& toCPU, MemoryBus& toMemory);
	};


	template class VectorMemoryController<CORE_ID_Core0>;
	template class VectorMemoryController<CORE_ID_Core1>;
	template class VectorMemoryController<CORE_ID_Core2>;
	template class VectorMemoryController<CORE_ID_Core3>;
	template class VectorMemoryController<CORE_ID_Core4>;
	template class VectorMemoryController<CORE_ID_Core5>;
	template class VectorMemoryController<CORE_ID_Core6>;
	template class VectorMemoryController<CORE_ID_Core7>;
}