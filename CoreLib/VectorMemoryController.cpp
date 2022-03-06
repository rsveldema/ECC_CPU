#include "CoreLib.h"

namespace ecc
{
	METHOD_SECTION;

	template <CoreID core_id>
	ReturnObject VectorMemoryController<core_id>::run(VecMemoryBus &toCPU, MemoryBus &toMemory)
	{
		uint32_t i = 0;

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
					pkt.packet_type = VEC_BUS_PKT_TYPE_read_response_vec64;

					i = 0;
					while (i < NUMBER_OF_VECTOR_THREADS_INT64)
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
								BusPacket mem_pkt = toMemory.get_response();

								CONTEXT_SWITCH();

								assert(mem_pkt.source.core_id == pkt.source.core_id);
								assert(mem_pkt.source.within_core_id == pkt.source.within_core_id);
								//assert(mem_pkt.address == pkt.address.data[i]);

								$display("read value ", mem_pkt.payload, " for address ", pkt.address.data[i]);

								pkt.payload.data[i] = mem_pkt.payload;
								break;
							}
							CONTEXT_SWITCH();
						}

						i++;
					}

					toCPU.send_response(pkt);
				}
				else
				{
					assert(pkt.packet_type == VEC_BUS_PKT_TYPE_write_vec64);
					i = 0;
					while (i < NUMBER_OF_VECTOR_THREADS_INT64)
					{
						while (toMemory.request_busy)
						{
							CONTEXT_SWITCH();
						}
						$display("store vec value ", get(pkt.payload, i), " for address ", pkt.address.data[i]);
						toMemory.send_write_request_data(pkt.address.data[i], pkt.source, get(pkt.payload, i));
						i++;
					}
				}

				//}
			}

			CONTEXT_SWITCH();
		}
	}
}
