#include "CoreLib.h"


namespace ecc
{
	ReturnObject VectorMemoryController::run()
	{
		while (1)
		{
			if (toCPU.request_busy)
			{
				auto pkt = toCPU.accept_request();
				const auto& addresses = pkt.address;
				const auto& src = pkt.source;
				assert(areAllValidMemoryAddresses(addresses));

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

					switch (pkt.type)
					{
					default: abort();
					case VEC_BUS_PKT_TYPE_read_vec64:
					{
						VecBusPacket reply{
							.type = VEC_BUS_PKT_TYPE_read_response_vec64
						};

						for (unsigned i = 0; i < NUMBER_OF_VECTOR_THREADS_INT64; i++)
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
									reply.payload.data[i] = value;
									break;
								}
								CONTEXT_SWITCH();
							}
						}

						toCPU.send_response(reply);
						break;
					}

					case VEC_BUS_PKT_TYPE_write_vec64:
					{
						const auto& arrayData = pkt.payload;

						for (unsigned i = 0; i < NUMBER_OF_VECTOR_THREADS_INT64; i++)
						{
							while (toMemory.request_busy)
							{
								CONTEXT_SWITCH();
							}
							bus_packet_payload_t payload = get(arrayData, i);
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
