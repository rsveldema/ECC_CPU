#include "CoreLib.h"

namespace ecc
{
	METHOD_SECTION;

	template <CoreID core_id>
	ReturnObject VectorMemoryController<core_id>::run(VecMemoryBus &toCPU, MemoryBus &toMemory)
	{
		while (1)
		{
			if (toCPU.request_busy)
			{
				VecBusPacket pkt = toCPU.accept_request();
				CONTEXT_SWITCH();

				assert(areAllValidMemoryAddresses(pkt.address));

				/*
				if (false) //addresses.all_equal())
				{
					// all threads want to access the same memory location
					abort();
				}
				else if (addresses.are_all_adjacent_memory_addresses(4))
				{
					// burst read/write
					abort();
				}
				else
				{
				*/
				// all vec-threads want to access a different memory location

				if (pkt.packet_type == VEC_BUS_PKT_TYPE_read_vec64)
				{
					VecBusPacket reply = {
						VEC_BUS_PKT_TYPE_read_response_vec64};

					for (uint32_t i = 0; i < NUMBER_OF_VECTOR_THREADS_INT64; i++)
					{
						// wait for the bus to clear so we can send another request
						while (toMemory.request_busy)
						{
							CONTEXT_SWITCH();
						}
						toMemory.send_read_request_data(pkt.address.data[i], pkt.source);

						// wait for response:
						while (1)
						{
							if (toMemory.response_busy)
							{
								auto pkt = toMemory.get_response();

								reply.payload.data[i] = pkt.payload;
								break;
							}
							CONTEXT_SWITCH();
						}
					}

					toCPU.send_response(reply);
				}
				else
				{
					if (pkt.packet_type == VEC_BUS_PKT_TYPE_write_vec64)
					{
						for (uint32_t i = 0; i < NUMBER_OF_VECTOR_THREADS_INT64; i++)
						{
							while (toMemory.request_busy)
							{
								CONTEXT_SWITCH();
							}
							toMemory.send_write_request_data(pkt.address.data[i], pkt.source, get(pkt.payload, i));
						}
					}
					else
					{
						$display("unrecnognized pkt type in vector-memory-controller");
						assert(false);
					}
				}

				//}
			}

			CONTEXT_SWITCH();
		}
	}
}
