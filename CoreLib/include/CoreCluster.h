#pragma once

#include "MachineConfig.h"

namespace Simulator
{
	class CoreCluster
	{
	public:
		std::vector<std::unique_ptr<Core>> cores;
		//RAM idle_code;

		Multiplexer<RawMemoryBus> main_memory_data_access_multiplexer;
		Multiplexer<InsnCacheMemoryBus> main_memory_insn_access_multiplexer;
		RawMemoryBus sram_multiplexer_bus;

		CoreCluster(SimComponentRegistry& registry, RawMemoryBus& _data_memory_bus, InsnCacheMemoryBus& _insn_memory_bus, GlobalStats& globalStats,
			const MachineConfig& config)
			: main_memory_data_access_multiplexer(registry, _data_memory_bus),
			main_memory_insn_access_multiplexer(registry, _insn_memory_bus)
		{
			for (unsigned i = 0; i < config.num_cores; i++)
			{
				const auto core_id = static_cast<MachineInfo::CoreID>(i);
				cores.emplace_back(std::make_unique<Core>(registry, core_id, globalStats));

				main_memory_data_access_multiplexer.addInput(&cores[i]->getExternalDataBus(),
					[core_id](const RawMemoryBus::Packet& pkt) {
						return pkt.source.core_id == core_id;
					});

				main_memory_insn_access_multiplexer.addInput(&cores[i]->getExternalInsnBus(),
					[core_id](const InsnCacheMemoryBus::Packet& pkt) {
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
