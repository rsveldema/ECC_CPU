#pragma once

#include <memory>
#include <queue>
#include <optional>
#include <array>
#include <variant>
#include <cassert>

#include "MachineInfo.h"
#include "VectorValue.h"
#include "Packets.h"


namespace ecc
{
	// simple memory bus that allows sending requests and receiving replies.	 
	INTERFACE MemoryBus
	{
		//bool did_ini;

		bool request_busy;
		bool response_busy;

		BusPacket request_data;
		BusPacket response_data;

		METHOD_SECTION;

		void init()
		{			
			//did_init = true;
			request_busy = false;
			response_busy = false;
		}

		void send_read_request_data(memory_address_t address, const BusID &source)
		{
			//assert(did_init);
			//send_request(create_bus_packet(bus_read_data, 
			//       source, address, 0));

			request_data.packet_type = bus_read_data;
			request_data.source = source;
			request_data.address = address;
			request_data.payload = 0;

			request_busy = true;
		}

		void send_write_request_data(memory_address_t address,
									 const BusID &source,
									 const bus_packet_payload_t &payload_data)
		{
			//send_request(create_bus_packet(bus_write_data, source, address, payload_data));			

			request_data.packet_type = bus_write_data;
			request_data.source = source;
			request_data.address = address;
			request_data.payload = payload_data;

			request_busy = true;
		}

		void send_read_response(const bus_packet_payload_t &value, const BusID &source)
		{
			//	std::cerr << "READ RESP: " << value << "\n";			

			response_data.packet_type = bus_read_response;
			response_data.source = source;
			response_data.address = 0;
			response_data.payload = value;

			response_busy = true;
		}

		BusPacket accept_request()
		{	
			//assert(did_init);

			assert(request_busy);
			request_busy = false;
			return request_data;
		}

		BusPacket get_response()
		{
			//assert(did_init);
			assert(response_busy);
			response_busy = false;
			return response_data;
		}

		void send_request(const BusPacket &pkt)
		{
			assert(!request_busy);
			request_data = pkt;
			request_busy = true;
		}

		void send_response(const BusPacket &pkt)
		{
			assert(!response_busy);
			response_data = pkt;
			response_busy = true;
		}
	};

	METHOD_SECTION;

}