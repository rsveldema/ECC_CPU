#include "RAM.h"

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
				case MemoryBus::Type::read:
				{
					const auto address = pkt.address;
					assert(address >= 0);
					assert(address < (storage.size() - 8));

					MemoryBus::payload_t value;

					std::copy(storage.begin() + address, storage.begin() + address + pkt.size, value.begin());

					for (uint64_t i = 0; i < config.read_latency.cycles; i++)
					{
						Task& t = *this;
						co_await t;
					}

					toCPU.send_read_response(value, pkt.source, pkt.size);
					break;
				}

				case MemoryBus::Type::write:
				{
					const auto address = pkt.address;
					printf("access to address %ld\n", address);
					assert(address >= 0);
					assert(address < (storage.size() - 8));

					for (uint64_t i = 0; i < config.write_latency.cycles; i++)
					{
						Task& t = *this;
						co_await t;
					}

					std::copy(pkt.payload.begin(), pkt.payload.end(), storage.begin() + address);
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