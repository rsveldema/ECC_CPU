#pragma once

#include <optional>
#include <variant>

#include "MachineInfo.h"
#include "RegisterFile.h"

namespace ecc
{

	INTERFACE DecodeToExecuteBus
	{
		bool is_busy;
		DecodeExecPacket data;

		METHOD_SECTION;

		void init()
		{
			is_busy = false;
		}

		void send_req0(execution_mask_t exec_mask, 
					memory_address_t PC,
					ExecuteStageOpcode opcode)
		{
			assert(!is_busy);

			data.exec_mask = exec_mask;
			data.PC = PC;
			data.opcode = opcode;

			is_busy = true;
		}

		void send_req1(execution_mask_t exec_mask, 
					memory_address_t PC,
					ExecuteStageOpcode opcode,
					const VectorValue& value0)
		{
			assert(!is_busy);

			data.exec_mask = exec_mask;
			data.PC = PC;
			data.opcode = opcode;
			data.value0.vec = value0;

			is_busy = true;
		}

		void send_req2_reg(execution_mask_t exec_mask, 
					memory_address_t PC,
					ExecuteStageOpcode opcode,
					RegisterID value0,
					VectorValue value1)
		{
			assert(!is_busy);

			data.exec_mask = exec_mask;
			data.PC = PC;
			data.opcode = opcode;
			data.value0.regID = value0;
			data.value1 = value1;

			is_busy = true;
		}

		void send_req2_vec(execution_mask_t exec_mask, 
					memory_address_t PC,
					ExecuteStageOpcode opcode,
					VectorValue value0,
					VectorValue value1)
		{
			assert(!is_busy);

			data.exec_mask = exec_mask;
			data.PC = PC;
			data.opcode = opcode;
			data.value0.vec = value0;
			data.value1 = value1;

			is_busy = true;
		}



		void send_req3_reg(execution_mask_t exec_mask, 
					memory_address_t PC,
					ExecuteStageOpcode opcode,
					RegisterID value0,
					VectorValue value1,
					VectorValue value2)
		{
			assert(!is_busy);

			data.exec_mask = exec_mask;
			data.PC = PC;
			data.opcode = opcode;
			data.value0.regID = value0;
			data.value1 = value1;
			data.value2 = value2;

			is_busy = true;
		}

		void send_req3_vec(execution_mask_t exec_mask, 
					memory_address_t PC,
					ExecuteStageOpcode opcode,
					VectorValue value0,
					VectorValue value1,
					VectorValue value2)
		{
			assert(!is_busy);

			data.exec_mask = exec_mask;
			data.PC = PC;
			data.opcode = opcode;
			data.value0.vec = value0;
			data.value1 = value1;
			data.value2 = value2;

			is_busy = true;
		}

	

		DecodeExecPacket recv()
		{
			is_busy = false;
			return data;
		}
	};
}