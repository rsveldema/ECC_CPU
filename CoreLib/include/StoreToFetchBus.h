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

		void send(execution_mask_t mask, memory_address_t new_pc)

		{
			assert(!can_receive);
			data.exec_mask = mask;
			data.newpc = new_pc;
			can_receive = true;
		}

		StoreToFetchPacket recv()
		{
			assert(can_receive);
			can_receive = false;
			return data;
		}

	};
}