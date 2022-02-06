#pragma once

#include <optional>
#include <queue>
#include <cassert>
#include <cstdint>

#include "Defines.h"

#include "Packets.h"

namespace ecc
{

	INTERFACE FetchToDecodeBus
	{
		bool is_busy = false;
		FetchToDecodeBusPacket data;

		METHOD_SECTION;

		void send(const FetchToDecodeBusPacket& pkt)
		{
			assert(!is_busy);
			data = pkt;
			is_busy = true;
		}

		FetchToDecodeBusPacket recv()
		{
			assert(is_busy);
			FetchToDecodeBusPacket v = data;
			is_busy = false;			
			return v;
		}
	};
}