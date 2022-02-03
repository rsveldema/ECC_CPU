#include "RAM.h"
#include <cstring>

namespace ecc
{
	ecc::ReturnObject RAM::run()
	{
		while (1)
		{
			if (toCPU.request_busy)
			{
				const auto pkt = toCPU.accept_request();

				switch (pkt.type)
				{
				case MemoryBus::Type::read_data:
				{
					const auto address = pkt.address;
					assert(address >= 0);
					assert(address < (storage.size() - 8));

					auto* ptr = reinterpret_cast<MemoryBus::payload_t*>(storage.data() + address);
					MemoryBus::payload_t ret = *ptr;

					for (uint64_t i = 0; i < config.read_latency.cycles; i++)
					{
						CONTEXT_SWITCH();
					}

					toCPU.send_read_response(ret, pkt.source);
					break;
				}

				case MemoryBus::Type::write_data:
				{
					const auto address = pkt.address;

					const auto& value = pkt.payload;

					for (uint64_t i = 0; i < config.write_latency.cycles; i++)
					{
						CONTEXT_SWITCH();
					}

					auto* dest_ptr = reinterpret_cast<MemoryBus::payload_t*>(storage.data() + address);

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