#include "CoreLib.h"


namespace ecc
{
	ReturnObject VectorMemoryController::run()
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
					case VecBusPacketType::read_vec64:
					{
						VecBusPacket reply{
							.type = VecBusPacketType::read_response_vec64
						};

						for (unsigned i = 0; i < addresses.data.size(); i++)
						{
							// wait for the bus to clear so we can send another request
							while (toMemory.request_busy)
							{
								CONTEXT_SWITCH();
							}
							const auto address = addresses.data[i];
							toMemory.send_read_request_data(address, src);

							while (1)
							{
								if (toMemory.response_busy)
								{
									auto pkt = toMemory.get_response();
								
									const int64_t value = pkt.payload;
									reply.payload.set(i, value);
									break;
								}
								CONTEXT_SWITCH();
							}
						}

						toCPU.send_response(reply);
						break;
					}

					case VecBusPacketType::write_vec64:
					{
						const auto& arrayData = pkt->payload;

						for (unsigned i = 0; i < addresses.data.size(); i++)
						{
							while (toMemory.request_busy)
							{
								CONTEXT_SWITCH();
							}
							bus_packet_payload_t payload = arrayData.get(i);
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
