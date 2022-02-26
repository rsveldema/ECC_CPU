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
	
		bool is_store_to_pc;

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
					RegisterID reg_id,
					const VectorValue& vec,
					bool store_to_pc)
		{
			assert(!is_busy);
			data.exec_mask = exec_mask;
			data.PC = PC;
			data.opcode = opcode;
			data.dest.regID = reg_id;
			data.src.value = vec;
			data.is_store_to_pc = store_to_pc;
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

		void send_address(execution_mask_t exec_mask, 
					memory_address_t PC,
					StorageStageOpcode opcode,
					const memory_address_t& addr)
		{
			assert(!is_busy);
			data.exec_mask = exec_mask;
			data.PC = PC;
			data.opcode = opcode;
			data.dest.address = addr;
			data.is_store_to_pc = false;
			is_busy = true;
		}


		void send_2_address(execution_mask_t exec_mask, 
					memory_address_t PC,
					StorageStageOpcode opcode,
					const memory_address_t& addr1,
					const memory_address_t& addr2,
					execution_mask_t true_mask1,
					execution_mask_t false_mask2)
		{
			assert(!is_busy);
			data.exec_mask = exec_mask;
			data.PC = PC;
			data.opcode = opcode;
			data.dest.address = addr1;
			data.src.address = addr2;
			data.is_store_to_pc = false;
			data.execution_flags_true = true_mask1;
			data.execution_flags_false = false_mask2;
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