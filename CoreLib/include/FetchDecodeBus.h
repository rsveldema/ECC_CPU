#pragma once

#include <optional>
#include <queue>
#include <cassert>
#include <cstdint>

#include "Defines.h"

#include "Packets.h"

namespace ecc
{

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