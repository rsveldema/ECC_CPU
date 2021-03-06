#include <cstring>

#include "CoreLib.h"

namespace ecc
{
	ReturnObject L2InsnCache::run()
	{
		running = true;
		while (1)
		{
			if (toCPU.request_busy)
			{
				auto pkt = toCPU.accept_request();
				// translates from an InsnCachePacket to to an RawDataPacket
				memory_address_t addr = pkt.address;
				BusPacket rawPkt{
					.packet_type = bus_read_data,
					.source = pkt.source,
					.address = addr,
					.payload = pkt.address
				};

				toMemory.send_request(rawPkt);
			}

			if (toMemory.response_busy)
			{
				auto pkt = toMemory.get_response();
			
				BusPacket insn_cache_pkt{
					.packet_type = bus_read_response,
					.source = pkt.source,
					.address = pkt.address,
					.payload = pkt.payload
				};

				toCPU.send_response(insn_cache_pkt);
			}

			CONTEXT_SWITCH();
		}
		running = false;
	}
}