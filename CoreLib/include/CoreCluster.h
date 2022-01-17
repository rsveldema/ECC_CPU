#pragma once

#include "MachineConfig.h"

namespace Simulator
{
	class CoreCluster
	{
	public:
		std::vector<std::unique_ptr<Core>> cores;
		//RAM idle_code;

		Multiplexer main_memory_access_multiplexer;
		MemoryBus sram_multiplexer_bus;
		Multiplexer idle_core_sram_access_multiplexer;

		CoreCluster(SimComponentRegistry& registry, MemoryBus& _memory_bus, GlobalStats& globalStats,
			const MachineConfig& config)
			: main_memory_access_multiplexer(registry, _memory_bus),
			//idle_code(registry, sram_multiplexer_bus, config.idle_memory_size, config.sram_memory_read_latency, config.sram_memory_write_latency),
			idle_core_sram_access_multiplexer(registry, sram_multiplexer_bus)
		{
			for (unsigned i = 0; i < config.num_cores; i++)
			{
				const auto core_id = static_cast<MachineInfo::CoreID>(i);
				cores.emplace_back(std::make_unique<Core>(registry, core_id, globalStats));

				main_memory_access_multiplexer.addInput(&cores[i]->getExternalMemoryBus(), [core_id](const MemoryBus::Packet& pkt) {
					return pkt.source.core_id == core_id;
					});

				idle_core_sram_access_multiplexer.addInput(&cores[i]->getIdleCoreMemoryBus(), [core_id](const MemoryBus::Packet& pkt) {
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
