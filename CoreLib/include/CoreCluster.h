#pragma once

#include "MachineConfig.h"
#include "MemoryBus.h"
#include "Core.h"

namespace ecc
{
	class CoreCluster
	{
	public:
		std::vector<std::unique_ptr<Core>> cores;
		//RAM idle_code;

		Multiplexer<MemoryBus> main_memory_data_access_multiplexer;
		Multiplexer<MemoryBus> main_memory_insn_access_multiplexer;
		MemoryBus sram_multiplexer_bus;

		CoreCluster(SimComponentRegistry& registry, 
					MemoryBus& _data_memory_bus, 
					MemoryBus& _insn_memory_bus, 
					const MachineConfig& config)
			: main_memory_data_access_multiplexer(registry, _data_memory_bus),
			main_memory_insn_access_multiplexer(registry, _insn_memory_bus)
		{
			sram_multiplexer_bus.init();
	
			for (unsigned i = 0; i < config.num_cores; i++)
			{
				const auto core_id = static_cast<ecc::CoreID>(i);
				cores.emplace_back(std::make_unique<Core>(registry, core_id));

				main_memory_data_access_multiplexer.addInput(&cores[i]->getExternalDataBus(),
					[core_id](const BusPacket& pkt) {
						return pkt.source.core_id == core_id;
					});

				main_memory_insn_access_multiplexer.addInput(&cores[i]->getExternalInsnBus(),
					[core_id](const BusPacket& pkt) {
						return pkt.source.core_id == core_id;
					});

			}
		}

		bool hasHalted() const
		{
			for (unsigned i = 0; i < cores.size(); i++)
			{
				if (cores[i]->hasHalted())
				{
					return true;
				}
			}
			return false;
		}
	};
}
