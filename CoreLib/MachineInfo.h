#pragma once

#include <map>
#include <string>

namespace MachineInfo
{
	enum class Register
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

		MAX_REG_ID
	};

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

		LOAD_REG_REG,        // rX = [rY + offset] 
		STORE_REG_CONST_REG, // [rY + offset] = rX

		CMP8_REG_REG,         // flags = rX <> rY
		CMP16_REG_REG,        // flags = rX <> rY
		CMP32_REG_REG,        // flags = rX <> rY
		CMP64_REG_REG,        // flags = rX <> rY

		ADD8_REG_REG_REG,     // rX = rY + rZ
		ADD8_REG_REG_CONST,   // rX = rY + const

		ADD16_REG_REG_REG,     // rX = rY + rZ
		ADD16_REG_REG_CONST,   // rX = rY + const

		ADD32_REG_REG_REG,     // rX = rY + rZ
		ADD32_REG_REG_CONST,   // rX = rY + const

		ADD64_REG_REG_REG,     // rX = rY + rZ
		ADD64_REG_REG_CONST,   // rX = rY + const

		RSHIFT_REG_REG_REG,     // rX = rY << rZ
		RSHIFT_REG_REG_CONST,   // rX = rY << const

		L_SSHIFT_REG_REG_REG,     // rX = rY >>> rZ
		L_SSHIFT_REG_REG_CONST,   // rX = rY >>> const

		L_USHIFT_REG_REG_REG,     // rX = rY >> rZ
		L_USHIFT_REG_REG_CONST,   // rX = rY >> const

		MUL8_REG_REG_REG,     // rX = rY * rZ
		MUL8_REG_REG_CONST,   // rX = rY * const

		MUL16_REG_REG_REG,     // rX = rY * rZ
		MUL16_REG_REG_CONST,   // rX = rY * const

		MUL32_REG_REG_REG,     // rX = rY * rZ
		MUL32_REG_REG_CONST,   // rX = rY * const

		MUL64_REG_REG_REG,     // rX = rY * rZ
		MUL64_REG_REG_CONST,   // rX = rY * const

		DIV8_REG_REG_REG,     // rX = rY / rZ
		DIV8_REG_REG_CONST,   // rX = rY / const

		DIV16_REG_REG_REG,     // rX = rY / rZ
		DIV16_REG_REG_CONST,   // rX = rY / const

		DIV32_REG_REG_REG,     // rX = rY / rZ
		DIV32_REG_REG_CONST,   // rX = rY / const

		DIV64_REG_REG_REG,     // rX = rY / rZ
		DIV64_REG_REG_CONST,   // rX = rY / const

		JMP,         // PC += CONST 

		RESTORE_PC,     // PC = [rX + offset]
		MOVE_PCREL_REG_CONST16,

	};

	static constexpr auto INSTRUCTION_SIZE = 4;

	struct InstructionInfo
	{
		const Opcode opcode;
	};

	extern std::map<std::string, InstructionInfo> insnInfo;
	extern std::map<std::string, Register> regnames;
}