#include "CoreLib.h"


namespace ecc
{
	ecc::ReturnObject VectorMemoryController::run()
	{
		while (1)
		{
			if (auto pkt = toCPU.try_accept_request())
			{
				const auto& addresses = pkt->address;
				const auto& src = pkt->source;
				assert(addresses.areAllValidMemoryAddresses());

				if (false) //addresses.all_equal())
				{
					// all threads want to access the same memory location
					abort();
				}
				/*
				else if (addresses.are_all_adjacent_memory_addresses(4))
				{
					// burst read/write
					abort();
				}
				*/
				else
				{
					// all vec-threads want to access a different memory location

					switch (pkt->type)
					{
					default: abort();
					case VecMemoryBus::Type::read_vec64:
					{
						VecMemoryBus::Packet reply{
							.type = VecMemoryBus::Type::read_response
						};
						reply.payload = VectorValue(vec_vector_obj_t<int64_t>());

						for (unsigned i = 0; i < addresses.size(); i++)
						{
							while (toMemory.is_busy())
							{
								CONTEXT_SWITCH();
							}
							const auto address = addresses.data[i];
							toMemory.send_read_request_data(address, src);

							while (1)
							{
								if (auto pkt = toMemory.try_accept_response())
								{
									const int64_t value = pkt->payload;
									reply.payload.set(i, value);
									break;
								}
								CONTEXT_SWITCH();
							}
						}

						toCPU.send_response(reply);
						break;
					}

					case VecMemoryBus::Type::write_vec64:
					{
						assert(pkt->payload.getType() == VectorValue::Type::INT64);
						const auto& arrayData = pkt->payload.get_int64_array();

						for (unsigned i = 0; i < addresses.size(); i++)
						{
							while (toMemory.is_busy())
							{
								CONTEXT_SWITCH();
							}
							RawMemoryBus::payload_t payload = arrayData.get(i);
							const auto& address = addresses.data[i];
							toMemory.send_write_request_data(address, src, payload);
						}
						break;
					}
					}
				}
			}

			CONTEXT_SWITCH();
		}
	}
}
