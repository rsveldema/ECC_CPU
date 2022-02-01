#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <cstdint>
#include <array>

#include "Defines.h"

namespace ecc
{
	struct InstructionInfo
	{
		const Opcode opcode;
	};

	struct ExecuteStageInsnInfo
	{
		ExecuteStageOpcode opcode;
	};


	struct BusID
	{
		CoreID core_id;
		CoreComponentID within_core_id;
	};


	static BusID createBusID(CoreID core_id, CoreComponentID within_id)
	{
		return { core_id, within_id };
	}

	extern std::map<std::string, InstructionInfo> insnInfo;
	extern std::map<std::string, RegisterID> regnames;
	extern std::map<std::string, ExecuteStageInsnInfo> execInsnInfo;


	static std::string to_string(RegisterID r)
	{
		for (const auto& it : regnames)
		{
			if (it.second == r)
			{
				return it.first;
			}
		}
		return "unknown_register?";
	}

	static std::string to_string(Opcode op)
	{
		for (const auto& it : insnInfo)
		{
			if (it.second.opcode == op)
			{
				return it.first;
			}
		}
		return "unknown insn";
	}


	static std::string to_string(ExecuteStageOpcode op)
	{
		for (const auto& it : execInsnInfo)
		{
			if (it.second.opcode == op)
			{
				return it.first;
			}
		}
		return "unknown exec insn";
	}

	static std::string to_string(StorageStageOpcode op)
	{
		switch (op)
		{
		case ecc::StorageStageOpcode::NOP: return "nop";
		case ecc::StorageStageOpcode::STORE_VALUE_INTO_REG: return "value_to_reg";
		case ecc::StorageStageOpcode::STORE_REG_INTO_MEM: return "store_mem";
		case ecc::StorageStageOpcode::CJMP: return "cjmp";
		case ecc::StorageStageOpcode::HALT: return "halt";
		case ecc::StorageStageOpcode::JMP: return "jmp";
		case ecc::StorageStageOpcode::LOAD_MEM_INTO_REG: return "load_reg";

		}
		return "unknown store type";
	}

	static std::string to_string(ecc::CoreID core_id)
	{
		return "core[" + std::to_string((int)core_id) + "]";
	}
}