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
				case MemoryBus::Type::read_insn:
				{
					const auto address = pkt.address;
					assert(address >= 0);
					assert(address < (storage.size() - 8));

					MemoryBus::payload_t value;

					MachineInfo::instruction_t ret;
					auto* ptr = storage.data() + address;
					memcpy(&ret, ptr, sizeof(ret));


					for (uint64_t i = 0; i < config.read_latency.cycles; i++)
					{
						Task& t = *this;
						co_await t;
					}

					toCPU.send_read_response(value, pkt.source);
					break;
				}

				case MemoryBus::Type::write_vec:
				{
					const auto address = pkt.address;
					//printf("access to address %ld\n", address);
					assert(address >= 0);
					assert(address < (storage.size() - 8));

					for (uint64_t i = 0; i < config.write_latency.cycles; i++)
					{
						Task& t = *this;
						co_await t;
					}

					auto* ptr = reinterpret_cast<VectorValue*>(storage.data() + address);
					*ptr = std::get<VectorValue>(pkt.payload);
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