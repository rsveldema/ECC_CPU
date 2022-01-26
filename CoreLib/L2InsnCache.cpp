#include "CoreLib.h"


namespace Simulator
{
	coro::ReturnObject L2InsnCache::run()
	{
		running = true;
		while (1)
		{
			if (auto pkt = toCPU.try_accept_request())
			{
				MachineInfo::memory_address_t addr = pkt->address;
				if ((addr & 7) == 4)
				{
					addr -= 4;
				}

				RawMemoryBus::Packet rawPkt{
					.type = RawMemoryBus::Type::read_data,
					.source = pkt->source,
					.address = addr,
					.payload = pkt->address
				};

				toMemory.send_request(rawPkt);
			}

			if (auto pkt = toMemory.try_accept_response())
			{
				InsnCacheMemoryBus::Packet insn_cache_pkt{
					.type = InsnCacheMemoryBus::Type::read_response,
					.source = pkt->source,
					.address = pkt->address,
					.payload = pkt->payload
				};

				toCPU.send_response(insn_cache_pkt);
			}

			Task& t = *this;
			co_await t;
		}
		running = false;
	}
}