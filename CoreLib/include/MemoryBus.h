#pragma once

#include <memory>
#include <queue>
#include <optional>
#include <array>
#include <variant>
#include <cassert>

#include "MachineInfo.h"
#include "VectorValue.h"


namespace ecc
{
		enum class BusPacketType
		{
			read_data,
			write_data,

			read_response,
			write_response
		};

		using bus_packet_payload_t = uint64_t;

		struct BusPacket
		{
			BusPacketType type;
			BusID source;
			memory_address_t address;
			bus_packet_payload_t payload;

			fetched_instruction_data_t getInsnData() 
			{
				assert(sizeof(fetched_instruction_data_t) == sizeof(payload));
				return *reinterpret_cast<fetched_instruction_data_t*>(&payload);
			}
		};



	/** simple memory bus that allows sending requests and receiving replies.
	*/
	class MemoryBus
	{
	public:
		using Type = BusPacketType;
		using Packet = BusPacket;
		using payload_t = bus_packet_payload_t;


		bool request_busy = false;
		bool response_busy = false;

		Packet request_data;
		Packet response_data;

		void send_read_request_data(memory_address_t address, const BusID& source)
		{
			Packet pkt{ Type::read_data, source, address };
			send_request(pkt);
		}


		void send_write_request_data(memory_address_t address,
			const BusID& source,
			const payload_t& value)
		{
			Packet pkt{ Type::write_data, source, address, value };
			send_request(pkt);
		}

		void send_read_response(const payload_t& value, const BusID& source)
		{
			memory_address_t addr = 0;
			Packet pkt{ Type::read_response, source, addr, value };
			send_response(pkt);
		}

		Packet accept_request()
		{
			const Packet f = request_data;
			request_busy = false;
			return f;			
		}

		Packet get_response()
		{
			assert(response_busy);
			const Packet f = response_data;
			response_busy = false;
			return f;
		}

		void send_request(const Packet& pkt)
		{
			assert(! request_busy);
			request_data = pkt;
			request_busy = true;
		}

		void send_response(const Packet& pkt)
		{
			response_data = pkt;
			response_busy = true;
		}
	};

}