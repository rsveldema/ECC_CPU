#pragma once

namespace Simulator
{
	class Cache : public SimComponent
	{
	public:
		MemoryBus& toCPU;
		MemoryBus& toMemory;

		Cache(SimComponentRegistry& registry,
			const std::string& name,
			MemoryBus& toCPU,
			MemoryBus& toMemory) : SimComponent(registry, name), toCPU(toCPU), toMemory(toMemory) {}

		coro::ReturnObject run() override
		{
			while (1)
			{
				if (auto pkt = toCPU.try_accept_request())
				{
					toMemory.send_request(*pkt);
				}

				if (auto pkt = toMemory.try_accept_response())
				{
					toCPU.send_response(*pkt);
				}

				co_await *this;
			}
		}
	};
}