#include "RAM.h"
#include <cstring>

namespace ecc
{
	ecc::ReturnObject RAM::run()
	{
		while (1)
		{
			if (const auto pkt_opt = toCPU.try_accept_request())
			{
				const auto& pkt = *pkt_opt;

				switch (pkt.type)
				{
				case RawMemoryBus::Type::read_data:
				{
					const auto address = pkt.address;
					assert(address >= 0);
					assert(address < (storage.size() - 8));

					auto* ptr = reinterpret_cast<RawMemoryBus::payload_t*>(storage.data() + address);
					RawMemoryBus::payload_t ret = *ptr;

					for (uint64_t i = 0; i < config.read_latency.cycles; i++)
					{
						CONTEXT_SWITCH();
					}

					toCPU.send_read_response(ret, pkt.source);
					break;
				}

				case RawMemoryBus::Type::write_data:
				{
					const auto address = pkt.address;

					const auto& value = pkt.payload;

					for (uint64_t i = 0; i < config.write_latency.cycles; i++)
					{
						CONTEXT_SWITCH();
					}

					auto* dest_ptr = reinterpret_cast<RawMemoryBus::payload_t*>(storage.data() + address);

					*dest_ptr = value;
					break;
				}

				default:
					abort();
				}
			}

			CONTEXT_SWITCH();
		}
	}
}