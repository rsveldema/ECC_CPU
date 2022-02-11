#pragma once

#include <optional>
#include <variant>

#include "MachineInfo.h"
#include "RegisterFile.h"

namespace ecc
{

	class DecodeToExecuteBus
	{
	public:
		struct Packet
		{
			execution_mask_t exec_mask;

			ecc::memory_address_t PC;
			ecc::ExecuteStageOpcode opcode;

			std::variant<ecc::RegisterID, VectorValue> value0;
			VectorValue value1;
			VectorValue value2;
		};

		bool is_busy;
		Packet data;

		void init()
		{
			is_busy = false;
		}

		void send(const Packet& pkt)
		{
			assert(!is_busy);
			data = pkt;
			is_busy = true;
		}

		Packet recv()
		{
			Packet tmp = data;
			is_busy = false;
			return tmp;
		}
	};
}