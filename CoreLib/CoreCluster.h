#pragma once

namespace Simulator
{
	class CoreCluster
	{
	public:
		std::vector<std::unique_ptr<Core>> cores;

		Multiplexer multiplexer;

		CoreCluster(SimComponentRegistry& registry, unsigned num_cores, MemoryBus& _memory_bus)
			: multiplexer(registry, _memory_bus)
		{
			for (unsigned i = 0; i < num_cores; i++)
			{
				const auto core_id = static_cast<MachineInfo::CoreID>(i);
				cores.emplace_back(std::make_unique<Core>(registry, core_id));

				multiplexer.addInput(&cores[i]->getExternalMemoryBus(), [core_id](const MemoryBus::Packet& pkt) {
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
