#include "RAM.h"
#include <cstring>

namespace Simulator
{
	coro::ReturnObject RAM::run()
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


					MachineInfo::instruction_t ret;
					auto* ptr = storage.data() + address;
					memcpy(&ret, ptr, sizeof(ret));

					RawMemoryBus::payload_t value(ret);

					for (uint64_t i = 0; i < config.read_latency.cycles; i++)
					{
						Task& t = *this;
						co_await t;
					}

					toCPU.send_read_response(value, pkt.source);
					break;
				}

				case RawMemoryBus::Type::write_data:
				{
					const auto address = pkt.address;

					const auto& value = pkt.payload;

					for (uint64_t i = 0; i < config.write_latency.cycles; i++)
					{
						Task& t = *this;
						co_await t;
					}

					auto* dest_ptr = reinterpret_cast<int64_t*>(storage.data() + address);

					*dest_ptr = value;
					break;
				}

				default:
					abort();
				}
			}

			Task& t = *this;
			co_await t;
		}
	}
}