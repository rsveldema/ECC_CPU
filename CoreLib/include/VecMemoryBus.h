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
	enum VecBusPacketType
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
	class VecMemoryBus
	{
	public:
		std::queue<VecBusPacket> request_queue;
		std::queue<VecBusPacket> response_queue;

		void send_read_request_vec(const VectorValue &address, const BusID &source)
		{
			VecBusPacket pkt{VEC_BUS_PKT_TYPE_read_vec64, source, address};
			send_request(pkt);
		}

		void send_write_request_vec(const VectorValue &address,
									const BusID &source,
									const VectorValue &value)
		{
			VecBusPacket pkt{VEC_BUS_PKT_TYPE_write_vec64,
							 source, address, value};
			send_request(pkt);
		}

		std::optional<VecBusPacket> try_accept_request()
		{
			if (!request_queue.empty())
			{
				const VecBusPacket f = request_queue.front();
				request_queue.pop();
				return f;
			}
			return std::nullopt;
		}

		std::optional<VecBusPacket> try_accept_response()
		{
			if (!response_queue.empty())
			{
				const VecBusPacket f = response_queue.front();
				response_queue.pop();
				return f;
			}
			return std::nullopt;
		}

		void send_request(const VecBusPacket &pkt)
		{
			assert(!is_busy());
			request_queue.push(pkt);
		}

		void send_response(const VecBusPacket &pkt)
		{
			response_queue.push(pkt);
		}

		bool is_busy() const
		{
			return request_queue.size() > 0;
		}
	};

}