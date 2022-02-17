#include "CoreLib.h"


namespace ecc
{
	std::array<uint8_t, END_MEMORY_ADDRESS> storage;

	METHOD_SECTION;

	void write_to_global_memory(uint64_t address, uint8_t data)
	{
		storage[address] = data;
	}

	ReturnObject DRAM::run()
	{
		while (1)
		{
			if (toCPU.request_busy)
			{
				const auto pkt = toCPU.accept_request();

				CONTEXT_SWITCH();

				switch (pkt.packet_type)
				{
				case bus_read_data:
				{
					assert(pkt.address >= 0);
					assert(pkt.address < (sizeof(storage) - 8));

					bus_packet_payload_t ret = 0;
					for (int i = 0; i < sizeof(bus_packet_payload_t); i++)
					{
						ret = ret | (static_cast<bus_packet_payload_t>(storage[pkt.address + i]) << (i * 8));
					}

					READ_MEMORY_DELAY();

					toCPU.send_read_response(ret, pkt.source);
					break;
				}

				case bus_write_data:
				{
					assert(pkt.address >= 0);
					assert(pkt.address < (sizeof(storage) - 8));

					WRITE_MEMORY_DELAY();

					bus_packet_payload_t ret = 0;
					for (int i = 0; i < sizeof(bus_packet_payload_t); i++)
					{
						storage[(pkt.address + i)] = (pkt.payload >> (i * 8)); 
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