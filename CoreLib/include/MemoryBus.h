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
	//	using Type = BusPacketType;
	//	using Packet = BusPacket;
	//	using payload_t = bus_packet_payload_t;

		bool request_busy = false;
		bool response_busy = false;

		BusPacket request_data;
		BusPacket response_data;

		void send_read_request_data(memory_address_t address, const BusID &source)
		{
			BusPacket pkt{BusPacketType::read_data, source, address};
			send_request(pkt);
		}

		void send_write_request_data(memory_address_t address,
									 const BusID &source,
									 const bus_packet_payload_t &value)
		{
			BusPacket pkt{BusPacketType::write_data, source, address, value};
			send_request(pkt);
		}

		void send_read_response(const bus_packet_payload_t &value, const BusID &source)
		{
			memory_address_t addr = 0;
			BusPacket pkt{BusPacketType::read_response, source, addr, value};
			send_response(pkt);
		}

		BusPacket accept_request()
		{
			assert(request_busy);
			const BusPacket f = request_data;
			request_busy = false;
			return f;
		}

		BusPacket get_response()
		{
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

}