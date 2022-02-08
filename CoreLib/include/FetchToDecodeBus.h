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
		bool is_busy;
		FetchToDecodeBusPacket data;

		METHOD_SECTION;

		void init_fetch_to_decode_bus()
		{
			is_busy = false;
		}

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