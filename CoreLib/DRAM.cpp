#include "CoreLib.h"


namespace ecc
{
	std::array<uint8_t, static_cast<uint32_t>(END_MEMORY_ADDRESS)> storage;

	METHOD_SECTION;

	void INITIAL_write_to_global_memory(uint64_t address, uint8_t data)
	{		
		storage[static_cast<phys_memory_address_t>(address)] = data;
	}

	ReturnObject DRAM::run(MemoryBus& toCPU)
	{
		while (1)
		{
			if (toCPU.request_busy)
			{
				const BusPacket pkt = toCPU.accept_request();

				CONTEXT_SWITCH();

				switch (pkt.packet_type)
				{
				case bus_read_data:
				{
					assert(pkt.address < (sizeof(storage) - 8));

					CONTEXT_SWITCH();
					READ_MEMORY_DELAY();
					toCPU.send_read_response(
							PACK8(
								storage[static_cast<phys_memory_address_t>(pkt.address + 0)],
								storage[static_cast<phys_memory_address_t>(pkt.address + 1)],
								storage[static_cast<phys_memory_address_t>(pkt.address + 2)],
								storage[static_cast<phys_memory_address_t>(pkt.address + 3)],
								storage[static_cast<phys_memory_address_t>(pkt.address + 4)],
								storage[static_cast<phys_memory_address_t>(pkt.address + 5)],
								storage[static_cast<phys_memory_address_t>(pkt.address + 6)],
								storage[static_cast<phys_memory_address_t>(pkt.address + 7)]), 
							pkt.source);
					break;
				}

				case bus_write_data:
				{
					assert(pkt.address < (sizeof(storage) - 8));

					WRITE_MEMORY_DELAY();

					for (uint64_t i = 0; i < sizeof(bus_packet_payload_t); i++)
					{
						storage[static_cast<phys_memory_address_t>(pkt.address + i)] = static_cast<uint8_t>(pkt.payload >> (i * 8)); 
					}
					break;
				}

				default:
					{
						$display("unrecognized bus command");
						abort();
					}
				}
			}

			CONTEXT_SWITCH();
		}
	}
}