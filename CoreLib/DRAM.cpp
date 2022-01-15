#include "DRAM.h"

namespace Simulator
{

	coro::ReturnObject DRAM::run()
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

					toCPU.send_read_response(value, pkt.source, pkt.size);
					break;
				}

				case MemoryBus::Type::write:
				{
					const auto address = pkt.address;
					assert(address >= 0);
					assert(address < (storage.size() - 8));

					std::copy(pkt.payload.begin(), pkt.payload.end(), storage.begin() + address);
					break;
				}

				default:
					abort();
				}
			}

			co_await *this;
		}
	}
}