#pragma once

#include <memory>
#include <queue>
#include <optional>
#include <array>
#include <cassert>
#include <variant>

#include "MachineInfo.h"
#include "VectorValue.h"


namespace ecc
{
	enum class InsnCachePacketType
	{
		read_insn,
		read_response
	};

	// memory-address on the request, instruction-t on the response
	using insn_cache_payload_t = std::variant<ecc::memory_address_t, ecc::fetched_instruction_data_t>;

	struct InsnCacheMemoryBusPacket
	{
		using payload_t = insn_cache_payload_t;

		InsnCachePacketType type;
		BusID source;
		memory_address_t address;
		payload_t payload;

		const fetched_instruction_data_t& getInsnData() const
		{
			return std::get<fetched_instruction_data_t>(payload);
		}
	};

	/** simple memory bus that allows sending requests and receiving replies.
	*/
	class InsnCacheMemoryBus
	{
	public:
		using payload_t = insn_cache_payload_t;
		using Packet = InsnCacheMemoryBusPacket;

		std::queue<InsnCacheMemoryBusPacket> request_queue;
		std::queue<InsnCacheMemoryBusPacket> response_queue;

		void send_read_request_insn(ecc::memory_address_t address, const ecc::BusID& source)
		{
			InsnCacheMemoryBusPacket pkt{ InsnCachePacketType::read_insn, source, address };
			send_request(pkt);
		}


		void send_read_response(const payload_t& value, const BusID& source)
		{
			memory_address_t addr = 0;
			InsnCacheMemoryBusPacket pkt{ InsnCachePacketType::read_response, source, addr, value };
			send_response(pkt);
		}

		std::optional<InsnCacheMemoryBusPacket> try_accept_request()
		{
			if (!request_queue.empty())
			{
				const InsnCacheMemoryBusPacket f = request_queue.front();
				request_queue.pop();
				return f;
			}
			return std::nullopt;
		}

		bool have_response() const
		{
			return !response_queue.empty();
		}

		InsnCacheMemoryBusPacket get_response()
		{
			const InsnCacheMemoryBusPacket f = response_queue.front();
			response_queue.pop();
			return f;
		}

		void send_request(const InsnCacheMemoryBusPacket& pkt)
		{
			assert(!is_busy());
			request_queue.push(pkt);
		}

		void send_response(const InsnCacheMemoryBusPacket& pkt)
		{
			response_queue.push(pkt);
		}

		bool is_busy() const
		{
			return request_queue.size() > 0;
		}
	};

}