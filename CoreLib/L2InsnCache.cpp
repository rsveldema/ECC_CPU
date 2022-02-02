#include <cstring>

#include "CoreLib.h"

namespace ecc
{
	ecc::ReturnObject L2InsnCache::run()
	{
		running = true;
		while (1)
		{
			if (auto pkt = toCPU.try_accept_request())
			{
				// translates from an InsnCachePacket to to an RawDataPacket
				ecc::memory_address_t addr = pkt->address;
				RawMemoryBus::Packet rawPkt{
					.type = RawMemoryBus::Type::read_data,
					.source = pkt->source,
					.address = addr,
					.payload = pkt->address
				};

				toMemory.send_request(rawPkt);
			}

			if (toMemory.have_response())
			{
				auto pkt = toMemory.get_response();
			
				ecc::fetched_instruction_data_t ret;

				static_assert(sizeof(ret) == sizeof(pkt.payload));
				memcpy(ret.data(), &pkt.payload, sizeof(ret));

				InsnCacheMemoryBus::Packet insn_cache_pkt{
					.type = InsnCachePacketType::read_response,
					.source = pkt.source,
					.address = pkt.address,
					.payload = ret
				};

				toCPU.send_response(insn_cache_pkt);
			}

			CONTEXT_SWITCH();
		}
		running = false;
	}
}