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
	using insn_cache_payload_t = std::variant<memory_address_t, fetched_instruction_data_t>;

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

		bool request_busy = false;
		bool response_busy = false;

		InsnCacheMemoryBusPacket request_data;
		InsnCacheMemoryBusPacket response_data;

		void send_read_request_insn(memory_address_t address, const BusID& source)
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

		InsnCacheMemoryBusPacket accept_request()
		{
			assert(request_busy);
			const InsnCacheMemoryBusPacket f = request_data;
			request_busy = false;
			return f;
		}

		InsnCacheMemoryBusPacket get_response()
		{
			assert(response_busy);
			const InsnCacheMemoryBusPacket f = response_data;
			response_busy = false;
			return f;
		}

		void send_request(const InsnCacheMemoryBusPacket& pkt)
		{
			assert(! request_busy);
			request_data = pkt;
			request_busy = true;
		}

		void send_response(const InsnCacheMemoryBusPacket& pkt)
		{
			assert(! response_busy);
			response_data = pkt;
			response_busy = true;
		}
	};

}