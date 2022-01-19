#pragma once

#include <map>
#include <string>

namespace MachineInfo
{
	static constexpr unsigned POINTER_SIZE = 8;

	enum class RegisterID
	{
		R0,
		R1,
		R2,
		R3,

		R4,
		R5,
		R6,
		R7,

		R8,
		R9,
		SP,
		FLAGS,

		PC,

		MAX_REG_ID
	};

	static const uint64_t FLAGS_MASK_HALT = (1 << 0);
	static const uint64_t FLAGS_MASK_EQ = (1 << 1);
	static const uint64_t FLAGS_MASK_GT = (1 << 2);
	static const uint64_t FLAGS_MASK_LT = (1 << 3);

	enum class CoreID
	{
		Core0,
		Core1,
		Core2,
		Core3,
		Core4,
		Core5,
		Core6,
		Core7,
	};

	enum class CoreComponentID
	{
		FETCH,
		DECODE,
		EXECUTE,
		STORE
	};


	enum class Opcode
	{
		NOP,
		HALT,

		MOVE_REG_REG, // rX = rY

		MOVE_REG_CPU_ID,  // rX = <CPU_ID> (32 bit)

		MOVE_REG_TIMESTAMP,  // rX = <TIMESTAMP> (64 bit)

		MOVE_REG_CONST16,  // rX = CONST (16 bit)

		MOVE_R0_CONST24A,  // r0  = CONST (24 bit)
		OR_R0_CONST_24B,   // r0 |= CONST << 24 bit
		OR_R0_CONST_24C,   // r0 |= CONST << 48 (64 - 48 = 16 bits)

		LOAD_REG_CONST_REG,       // rX = [rY + offset] 
		STORE_REG_CONST_REG,  // [rY + offset] = rX

		CMP_REG_REG,        // flags = rX <> rY

		ADD_REG_REG_REG,     // rX = rY + rZ
		ADD_REG_REG_CONST,   // rX = rY + const

		RSHIFT_REG_REG_REG,     // rX = rY << rZ
		RSHIFT_REG_REG_CONST,   // rX = rY << const

		L_SSHIFT_REG_REG_REG,     // rX = rY >>> rZ
		L_SSHIFT_REG_REG_CONST,   // rX = rY >>> const

		L_USHIFT_REG_REG_REG,     // rX = rY >> rZ
		L_USHIFT_REG_REG_CONST,   // rX = rY >> const

		MUL_REG_REG_REG,     // rX = rY * rZ
		MUL_REG_REG_CONST,   // rX = rY * const

		DIV_REG_REG_REG,     // rX = rY / rZ
		DIV_REG_REG_CONST,   // rX = rY / const

		JMP_ALWAYS,         // PC += CONST 
		JMP_EQUAL,
		JMP_NOT_EQUAL,
		JMP_LOWER,
		JMP_LOWER_EQUAL,
		JMP_GREATER,
		JMP_GREATER_EQUAL,

		LOAD_RESTORE_PC,     // PC = [rX + offset]
		MOVE_PCREL_REG_CONST16,
	};

	enum class ExecuteStageOpcode
	{
		NOP,
		MOVE_REG_VALUE,
		STORE_ADDR_VALUE,
		JMP,
		LOAD_RESTORE_PC,
		LOAD_REG,
		CMP,
		COND_JMP
	};

	enum class StorageStageOpcode
	{
		NOP,
		STORE_PC,
		STORE_REG,
		STORE_MEM,
		JMP,
		COND_JMP,		
		LOAD_REG
	};

	static constexpr auto INSTRUCTION_SIZE = 4;

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

	static bool changesControlFlow(Opcode op)
	{
		switch (op)
		{
		case Opcode::JMP_ALWAYS:
		case Opcode::JMP_EQUAL:
		case Opcode::JMP_NOT_EQUAL:
		case Opcode::JMP_GREATER:
		case Opcode::JMP_GREATER_EQUAL:
		case Opcode::JMP_LOWER:
		case Opcode::JMP_LOWER_EQUAL:
		case Opcode::LOAD_RESTORE_PC:
			return true;
		default:
			return false;
		}
		return false;
	}

	static std::string to_string(RegisterID r)
	{
		for (auto it : regnames)
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
		for (auto it : insnInfo)
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
		for (auto it : execInsnInfo)
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
		case MachineInfo::StorageStageOpcode::NOP: return "nop";
		case MachineInfo::StorageStageOpcode::STORE_PC: return "store-pc";
		case MachineInfo::StorageStageOpcode::STORE_REG: return "store_reg";
		case MachineInfo::StorageStageOpcode::STORE_MEM: return "store_mem";
		}
		return "unknown store type";
	}

	static std::string to_string(MachineInfo::CoreID core_id)
	{
		return "core[" + std::to_string((int)core_id) + "]";
	}
}