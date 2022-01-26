#pragma once

#include <memory>
#include <queue>
#include <optional>
#include <array>
#include <variant>

#include "MachineInfo.h"
#include "VectorValue.h"


namespace Simulator
{
	/** simple memory bus that allows sending requests and receiving replies.
	*/
	class VecMemoryBus
	{
	public:
		enum class Type
		{
			read_insn,
			read_vec64,
			write_vec64,
			read_response,
			write_response
		};

		using payload_t = VectorValue;

		struct Packet
		{
			Type type;
			MachineInfo::BusID source;
			vec_vector_obj_t<int64_t> address;
			payload_t payload;
		};


		std::queue<Packet> request_queue;
		std::queue<Packet> response_queue;

		void send_read_request_vec(const vec_vector_obj_t<int64_t>& address, const MachineInfo::BusID& source)
		{
			Packet pkt{ Type::read_vec64, source, address };
			send_request(pkt);
		}

		void send_write_request_vec(const vec_vector_obj_t<int64_t>& address,
			const MachineInfo::BusID& source,
			const VectorValue& value)
		{
			Packet pkt{ Type::write_vec64, source, address, value };
			send_request(pkt);
		}

		/*

		void send_read_request_insn(MachineInfo::memory_address_t address, const MachineInfo::BusID& source)
		{
			Packet pkt{ Type::read_insn, source, address };
			send_request(pkt);
		}

		void send_read_response(const payload_t& value, const MachineInfo::BusID& source)
		{
			MachineInfo::memory_address_t addr = 0;
			Packet pkt{ Type::read_response, source, addr, value };
			send_response(pkt);
		}
		*/

		std::optional<Packet> try_accept_request()
		{
			if (!request_queue.empty())
			{
				const Packet f = request_queue.front();
				request_queue.pop();
				return f;
			}
			return std::nullopt;
		}

		std::optional<Packet> try_accept_response()
		{
			if (!response_queue.empty())
			{
				const Packet f = response_queue.front();
				response_queue.pop();
				return f;
			}
			return std::nullopt;
		}

		void send_request(const Packet& pkt)
		{
			assert(!is_busy());
			request_queue.push(pkt);
		}

		void send_response(const Packet& pkt)
		{
			response_queue.push(pkt);
		}

		bool is_busy() const
		{
			return request_queue.size() > 0;
		}
	};

}