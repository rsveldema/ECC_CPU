#pragma once

#include <optional>
#include <variant>
#include <cassert>

#include "MachineInfo.h"
#include "VectorValue.h"

namespace ecc
{

	class ExecuteToStoreBus
	{
	public:
		struct Packet
		{
			execution_mask_t exec_mask;
			ecc::memory_address_t PC;
			ecc::StorageStageOpcode opcode;

			// in case its an address / register:
			std::variant<ecc::memory_address_t, VectorValue, ecc::RegisterID> dest;
			std::variant<ecc::memory_address_t, VectorValue, ecc::RegisterID> src;

			bool is_store_to_pc = false;

			execution_mask_t execution_flags_true;
			execution_mask_t execution_flags_false;
		};

		void send(const Packet& pkt)
		{
			assert(!is_busy);
			data = pkt;
			is_busy = true;
		}

		Packet recv()
		{
			assert(is_busy);
			Packet tmp = data;
			is_busy = false;
			return tmp;
		}

		bool is_busy  = false;
		Packet data;
	};
}