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
	enum VecBusPacketType: uint8_t
	{
		VEC_BUS_PKT_TYPE_read_vec64,
		VEC_BUS_PKT_TYPE_write_vec64,
		VEC_BUS_PKT_TYPE_read_response_vec64,
		VEC_BUS_PKT_TYPE_write_response_vec64
	};

	using vec_bus_payload_t = VectorValue;

	struct VecBusPacket
	{
		VecBusPacketType type;
		BusID source;
		VectorValue address;
		vec_bus_payload_t payload;
	};

	/** simple memory bus that allows sending requests and receiving replies.
	 */
	INTERFACE VecMemoryBus
	{
		bool request_busy;
		VecBusPacket request_queue;
		
		bool response_busy;
		VecBusPacket response_queue;

		METHOD_SECTION;

		void init()
		{
			request_busy = false;
			response_busy = false;
		}

		void send_read_request_vec(const VectorValue &address, const BusID &source)
		{
			assert(! request_busy);		
			request_queue.source = source;
			request_queue.address = address;
			request_queue.type = VEC_BUS_PKT_TYPE_read_vec64;
			request_busy = true;
		}

		void send_write_request_vec(const VectorValue &address,
									const BusID &source,
									const VectorValue &value)
		{
			request_queue.source = source;
			request_queue.address = address;
			request_queue.payload = value;
			request_queue.type = VEC_BUS_PKT_TYPE_write_vec64;
			request_busy = true;
		}

		VecBusPacket accept_request()
		{
			assert(request_busy);
			request_busy = false;
			return request_queue;
		}

		VecBusPacket accept_response()
		{
			assert(response_busy);
			response_busy = false;
			return response_queue;
		}

		void send_response(const VecBusPacket &pkt)
		{
			assert(!response_busy);
			response_queue = pkt;
			response_busy = true;
		}
	};

}