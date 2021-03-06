#pragma once

#include "MemoryBus.h"
#include "Core.h"
#include "CoreSelectionMemoryMultiplexer.h"

namespace ecc
{
	class CoreCluster
	{
	public:
		Core<CORE_ID_Core0> core0;
		//RAM idle_code;

		CoreSelectionMemoryMultiplexer main_memory_data_access_multiplexer;
		CoreSelectionMemoryMultiplexer main_memory_insn_access_multiplexer;
		MemoryBus sram_multiplexer_bus;

		void init()
		{
			core0.init();
			sram_multiplexer_bus.init();
		}

		CoreCluster(SimComponentRegistry& registry, 
					MemoryBus& _data_memory_bus, 
					MemoryBus& _insn_memory_bus)
			: core0(registry),
			  main_memory_data_access_multiplexer(registry, _data_memory_bus),
			  main_memory_insn_access_multiplexer(registry, _insn_memory_bus)
		{
			sram_multiplexer_bus.init();


			main_memory_data_access_multiplexer.addInput(0, &core0.getExternalDataBus());

			main_memory_insn_access_multiplexer.addInput(0, &core0.getExternalInsnBus());			
		}

		bool hasHalted() const
		{
			if (core0.hasHalted())
			{
				return true;
			}		
			return false;
		}
	};
}
