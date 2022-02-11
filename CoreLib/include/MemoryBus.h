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
		bool did_init = false;

		bool request_busy;
		bool response_busy;

		BusPacket request_data;
		BusPacket response_data;

		METHOD_SECTION;

		void init()
		{			
			did_init = true;
			request_busy = false;
			response_busy = false;
		}

		void send_read_request_data(memory_address_t address, const BusID &source)
		{
			assert(did_init);
			BusPacket pkt = create_bus_packet(bus_read_data, source, address, 0);
			send_request(pkt);
		}

		void send_write_request_data(memory_address_t address,
									 const BusID &source,
									 const bus_packet_payload_t &payload_data)
		{
			BusPacket pkt = create_bus_packet(bus_write_data, source, address, payload_data);
			send_request(pkt);
		}

		void send_read_response(const bus_packet_payload_t &value, const BusID &source)
		{
			memory_address_t addr = 0;
			BusPacket pkt = create_bus_packet(bus_read_response, source, addr, value);
			send_response(pkt);
		}

		BusPacket accept_request()
		{	
			assert(did_init);

			assert(request_busy);
			const BusPacket f = request_data;
			request_busy = false;
			return f;
		}

		BusPacket get_response()
		{
			assert(did_init);
			assert(response_busy);
			const BusPacket f = response_data;
			response_busy = false;
			return f;
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