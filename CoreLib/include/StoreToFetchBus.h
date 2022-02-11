#pragma once

#include <optional>
#include <cassert>

#include "MachineInfo.h"
#include "Packets.h"

namespace ecc
{

	INTERFACE StoreToFetchBus
	{
		bool can_receive;
		StoreToFetchPacket data;

		METHOD_SECTION;


		void init()
		{
			can_receive = false;
		}

		void init_store_to_fetch_bus()
		{
			can_receive = false;
		}

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

	};
}