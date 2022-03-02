#pragma once

#include "MachineConfig.h"
#include "MemoryBus.h"
#include "Core.h"

namespace ecc
{
	class CoreCluster
	{
	public:
		Core<CORE_ID_Core0> core0;
		//RAM idle_code;

		Multiplexer<MemoryBus> main_memory_data_access_multiplexer;
		Multiplexer<MemoryBus> main_memory_insn_access_multiplexer;
		MemoryBus sram_multiplexer_bus;

		void dump_stats()
		{
			dump(core0.stats);
		}

		void init()
		{
			core0.init();
			sram_multiplexer_bus.init();
		}

		CoreCluster(SimComponentRegistry& registry, 
					MemoryBus& _data_memory_bus, 
					MemoryBus& _insn_memory_bus, 
					const MachineConfig& config)
			: core0(registry),
			  main_memory_data_access_multiplexer(registry, _data_memory_bus),
			  main_memory_insn_access_multiplexer(registry, _insn_memory_bus)
		{
			sram_multiplexer_bus.init();


			main_memory_data_access_multiplexer.addInput(&core0.getExternalDataBus(),
				[](const BusPacket& pkt) {
					return pkt.source.core_id == CORE_ID_Core0;
				});

			main_memory_insn_access_multiplexer.addInput(&core0.getExternalInsnBus(),
				[](const BusPacket& pkt) {
					return pkt.source.core_id == CORE_ID_Core0;
				});			
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
