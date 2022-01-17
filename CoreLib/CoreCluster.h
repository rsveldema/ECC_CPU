#pragma once

namespace Simulator
{
	class Processor
	{
	public:
		std::vector<std::unique_ptr<Core>> cores;

		Processor(SimComponentRegistry& registry, unsigned num_cores, MemoryBus& memory_bus)
		{
			for (unsigned i = 0; i < num_cores; i++)
			{
				cores.emplace_back(std::make_unique<Core>(registry, memory_bus, static_cast<MachineInfo::CoreID>(i)));
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
