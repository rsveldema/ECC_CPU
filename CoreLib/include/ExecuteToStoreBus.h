#pragma once

#include <optional>
#include <cassert>

#include "MachineInfo.h"
#include "VectorValue.h"

namespace ecc
{
	union ExecStageValue
	{
		memory_address_t address;
		VectorValue value;
		RegisterID regID;
	};

	struct ExecStagePacket
	{
		execution_mask_t exec_mask;
		memory_address_t PC;
		StorageStageOpcode opcode;

		ExecStageValue dest;
		ExecStageValue src;
	
		bool is_store_to_pc = false;

		execution_mask_t execution_flags_true;
		execution_mask_t execution_flags_false;
	};

	INTERFACE ExecuteToStoreBus
	{
		bool is_busy;
		ExecStagePacket data;

		METHOD_SECTION;

		void init()
		{
			is_busy = false;
		}

		void send(const ExecStagePacket &pkt)
		{
			assert(!is_busy);
			data = pkt;
			is_busy = true;
		}

		void send_none(execution_mask_t exec_mask, 
					memory_address_t PC,
					StorageStageOpcode opcode)
		{
			assert(!is_busy);
			data.exec_mask = exec_mask;
			data.PC = PC;
			data.opcode = opcode;
			data.is_store_to_pc = false;
			is_busy = true;
		}
		
		void send_reg_vec(execution_mask_t exec_mask, 
					memory_address_t PC,
					StorageStageOpcode opcode,
					RegisterID reg,
					const VectorValue& vec)
		{
			assert(!is_busy);
			data.exec_mask = exec_mask;
			data.PC = PC;
			data.opcode = opcode;
			data.dest.regID = reg;
			data.src.value = vec;
			data.is_store_to_pc = false;
			is_busy = true;
		}

		void send_vec_vec(execution_mask_t exec_mask, 
					memory_address_t PC,
					StorageStageOpcode opcode,
					const VectorValue& vec1,
					const VectorValue& vec2)
		{
			assert(!is_busy);
			data.exec_mask = exec_mask;
			data.PC = PC;
			data.opcode = opcode;
			data.dest.value = vec1;
			data.src.value = vec2;
			data.is_store_to_pc = false;
			is_busy = true;
		}

		ExecStagePacket recv()
		{
			assert(is_busy);
			is_busy = false;
			return data;
		}
	};
}