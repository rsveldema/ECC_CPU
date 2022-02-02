#pragma once

#include <optional>
#include <queue>
#include <cassert>
#include <cstdint>

#include "Defines.h"

#include "ExecutionMask.h"

namespace ecc
{

	struct FetchToDecodeBusPacket
	{
		ExecutionMask exec_mask;
		ecc::memory_address_t PC;
		uint32_t insn;
	};
	
	class FetchToDecodeBus
	{
	public:

		void send(const FetchToDecodeBusPacket& pkt)
		{
			assert(!is_busy());
			data = pkt;
			can_receive = true;
		}

		FetchToDecodeBusPacket recv()
		{
			assert(can_receive);
			FetchToDecodeBusPacket v = data;
			can_receive = false;			
			return v;
		}

		bool is_busy() const
		{
			return can_receive;
		}

		bool can_receive = false;
		FetchToDecodeBusPacket data;
	};
}