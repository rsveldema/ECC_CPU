#pragma once

#include <optional>
#include <cassert>

#include "MachineInfo.h"
#include "ExecutionMask.h"

namespace ecc
{
	struct StoreToFetchPacket
	{
		ExecutionMask exec_mask;
		memory_address_t newpc;
	};

	class StoreToFetchBus
	{
	public:
		void send(const StoreToFetchPacket& pkt)
		{
			assert(!can_receive);
			data = pkt;
			can_receive = true;
		}

		StoreToFetchPacket recv()
		{
			assert(can_receive);
			StoreToFetchPacket v = data;
			can_receive = false;
			return v;
		}

		bool can_receive = false;
		StoreToFetchPacket data;
	};
}