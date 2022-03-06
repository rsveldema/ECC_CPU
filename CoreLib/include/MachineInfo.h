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

	extern std::map<std::string, InstructionInfo> insnInfo;
	extern std::map<std::string, RegisterID> regnames;
	extern std::map<std::string, ExecuteStageInsnInfo> execInsnInfo;

}

#include "log_utils.h"

namespace ecc
{    
	static std::string to_string(const RegisterID& r)
	{
		for (const auto& it : regnames)
		{
			if (it.second == r)
			{
				return std::to_string(static_cast<int>(r)) + "/"  + it.first;
			}
		}
		return "unknown_register?";
	}

	static std::string to_string(const Opcode& op)
	{
		for (const auto& it : insnInfo)
		{
			if (it.second.opcode == op)
			{
				return std::to_string(static_cast<int>(op)) + "/"  + it.first;
			}
		}
		return "unknown insn";
	}


	static std::string to_string(const ExecuteStageOpcode& op)
	{
		for (const auto& it : execInsnInfo)
		{
			if (it.second.opcode == op)
			{
				return std::to_string(static_cast<int>(op)) + "/"  + it.first;
			}
		}
		return "unknown exec insn";
	}

	static std::string to_string(const StorageStageOpcode& op)
	{
		std::string ret;
		switch (op)
		{
		case StorageStageOpcode::STORAGE_NOP: ret = "nop"; break;
		case StorageStageOpcode::STORAGE_STORE_VALUE_INTO_REG: ret =  "value_to_reg"; break;
		case StorageStageOpcode::STORAGE_STORE_REG_INTO_MEM: ret = "store_mem"; break;
		case StorageStageOpcode::STORAGE_CJMP: ret =  "cjmp"; break;
		case StorageStageOpcode::STORAGE_HALT: ret =  "halt"; break;
		case StorageStageOpcode::STORAGE_JMP: ret =  "jmp"; break;
		case StorageStageOpcode::STORAGE_LOAD_MEM_INTO_REG: ret =  "load_reg"; break;
		default: ret = "unknown"; break;
		}
		auto prefix = std::to_string(static_cast<int>(op)) + "/";
		return prefix + ret;
	}

	static std::string to_string(const ecc::CoreID& core_id)
	{
		return "core[" + std::to_string((int)core_id) + "]";
	}
}