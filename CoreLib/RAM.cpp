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
				case MemoryBus::Type::read_insn:
				{
					const auto address = std::get<MachineInfo::memory_address_t>(pkt.address);
					assert(address >= 0);
					assert(address < (storage.size() - 8));


					MachineInfo::instruction_t ret;
					auto* ptr = storage.data() + address;
					memcpy(&ret, ptr, sizeof(ret));

					MemoryBus::payload_t value(ret);

					for (uint64_t i = 0; i < config.read_latency.cycles; i++)
					{
						Task& t = *this;
						co_await t;
					}

					toCPU.send_read_response(value, pkt.source);
					break;
				}

				case MemoryBus::Type::read_vec64:
				{
					const auto& addressVec = std::get< vec_vector_obj_t<int64_t>>(pkt.address);
					assert(addressVec.areAllValidMemoryAddresses());

					VectorValue ret = VectorValue::create_vec_int64(0);
					for (unsigned i = 0; i < addressVec.size(); i++)
					{
						MachineInfo::memory_address_t address = addressVec.get(i);
						const auto* src_ptr = reinterpret_cast<int64_t*>(storage.data() + address);

						ret.set_int64(i, *src_ptr);
					}

					MemoryBus::payload_t value(ret);

					for (uint64_t i = 0; i < config.read_latency.cycles; i++)
					{
						Task& t = *this;
						co_await t;
					}

					toCPU.send_read_response(value, pkt.source);
					break;
				}

				case MemoryBus::Type::write_vec64:
				{
					const auto& addressVec = std::get< vec_vector_obj_t<int64_t>>(pkt.address);
					assert(addressVec.areAllValidMemoryAddresses());

					const auto& value = std::get<VectorValue>(pkt.payload);


					for (uint64_t i = 0; i < config.write_latency.cycles; i++)
					{
						Task& t = *this;
						co_await t;
					}

					for (unsigned i = 0; i < addressVec.size(); i++)
					{
						MachineInfo::memory_address_t address = addressVec.get(i);
						auto* dest_ptr = reinterpret_cast<int64_t*>(storage.data() + address);

						auto value64 = value.get_int64(i);
						*dest_ptr = value64;
					}
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