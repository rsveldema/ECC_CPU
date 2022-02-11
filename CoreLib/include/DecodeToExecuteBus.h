#pragma once

#include <optional>
#include <variant>

#include "MachineInfo.h"
#include "RegisterFile.h"

namespace ecc
{
	union DecodeStageValue
	{
		RegisterID regID;
		VectorValue vec;
	};

	struct DecodeExecPacket
	{
		execution_mask_t exec_mask;

		memory_address_t PC;
		ExecuteStageOpcode opcode;

		DecodeStageValue value0;
		VectorValue value1;
		VectorValue value2;
	};

	class DecodeToExecuteBus
	{
	public:
		bool is_busy;
		DecodeExecPacket data;

		void init()
		{
			is_busy = false;
		}

		void send(const DecodeExecPacket &pkt)
		{
			assert(!is_busy);
			data = pkt;
			is_busy = true;
		}

		DecodeExecPacket recv()
		{
			DecodeExecPacket tmp = data;
			is_busy = false;
			return tmp;
		}
	};
}