#include "MachineInfo.h"

namespace MachineInfo
{

	std::map<std::string, ExecuteStageInsnInfo> execInsnInfo{
		{ "nop", {ExecuteStageOpcode::NOP}},
		{ "move_reg_value", {ExecuteStageOpcode::MOVE_REG_VALUE}},
	};


	std::map<std::string, InstructionInfo> insnInfo{
		{ "nop", { Opcode::NOP}},
		{ "halt", { Opcode::HALT}},

		{"movreg2reg", { Opcode::MOVE_REG_REG}}, // rX = rY

		{"move_reg_cpuid", { Opcode::MOVE_REG_CPU_ID}},  // rX = <CPU_ID> (32 bit)

		{"move_reg_TS", { Opcode::MOVE_REG_TIMESTAMP}},  // rX = <TIMESTAMP> (64 bit)

		{"move_reg_c16", { Opcode::MOVE_REG_CONST16}},  // rX = CONST (16 bit)

		{"move_r0_ca", { Opcode::MOVE_R0_CONST24A}},  // r0  = CONST (24 bit)
		{"move_r0_cb", { Opcode::OR_R0_CONST_24B}},   // r0 |= CONST << 24 bit
		{"move_r0_cc", { Opcode::OR_R0_CONST_24C}},   // r0 |= CONST << 48 (64 - 48 = 16 bits)

		{"load_r2r", { Opcode::LOAD_REG_REG}},        // rX = [rY + offset] 
		{"store_rcr", { Opcode::STORE_REG_CONST_REG}}, // [rY + offset] = rX

		{"cmpn_reg_reg", { Opcode::CMP8_REG_REG}},         // flags = rX <> rY
		{"cmps_reg_reg", { Opcode::CMP16_REG_REG}},        // flags = rX <> rY
		{"cmpw_reg_reg", { Opcode::CMP32_REG_REG}},        // flags = rX <> rY
		{"cmpl_reg_reg", { Opcode::CMP64_REG_REG}},        // flags = rX <> rY

		{"addb_rrr", { Opcode::ADD8_REG_REG_REG}},     // rX = rY + rZ
		{"addb_rrc", { Opcode::ADD8_REG_REG_CONST}},   // rX = rY + const

		{"adds_rrr", { Opcode::ADD16_REG_REG_REG}},     // rX = rY + rZ
		{"adds_rrc", { Opcode::ADD16_REG_REG_CONST}},   // rX = rY + const

		{"addw_rrr", { Opcode::ADD32_REG_REG_REG}},     // rX = rY + rZ
		{"addw_rrc", { Opcode::ADD32_REG_REG_CONST}},   // rX = rY + const

		{"addl_rrr", { Opcode::ADD64_REG_REG_REG}},     // rX = rY + rZ
		{"addl_rrc", { Opcode::ADD64_REG_REG_CONST}},   // rX = rY + const

		{"rs_rrr", { Opcode::RSHIFT_REG_REG_REG}},     // rX = rY << rZ
		{"rs_rrc", { Opcode::RSHIFT_REG_REG_CONST}},   // rX = rY << const

		{"ls_rrr", { Opcode::L_SSHIFT_REG_REG_REG}},     // rX = rY >>> rZ
		{"ls_rrc", { Opcode::L_SSHIFT_REG_REG_CONST}},   // rX = rY >>> const

		{"uls_rrr", { Opcode::L_USHIFT_REG_REG_REG}},     // rX = rY >> rZ
		{"uls_rrc", { Opcode::L_USHIFT_REG_REG_CONST}},   // rX = rY >> const

		{"mulb_rrr", { Opcode::MUL8_REG_REG_REG}},     // rX = rY * rZ
		{"mulb_rrc", { Opcode::MUL8_REG_REG_CONST}},   // rX = rY * const

		{"muls_rrr", { Opcode::MUL16_REG_REG_REG}},     // rX = rY * rZ
		{"muls_rrc", { Opcode::MUL16_REG_REG_CONST}},   // rX = rY * const

		{"mulw_rrr", { Opcode::MUL32_REG_REG_REG}},     // rX = rY * rZ
		{"mulw_rrc", { Opcode::MUL32_REG_REG_CONST}},   // rX = rY * const

		{"mull_rrr", { Opcode::MUL64_REG_REG_REG}},     // rX = rY * rZ
		{"mull_rrc", { Opcode::MUL64_REG_REG_CONST}},   // rX = rY * const

		{"divb_rrr", { Opcode::DIV8_REG_REG_REG}},     // rX = rY / rZ
		{"divb_rrc", { Opcode::DIV8_REG_REG_CONST}},   // rX = rY / const

		{"divs_rrr", { Opcode::DIV16_REG_REG_REG}},     // rX = rY / rZ
		{"divs_rrc", { Opcode::DIV16_REG_REG_CONST}},   // rX = rY / const

		{"divw_rrr", { Opcode::DIV32_REG_REG_REG}},     // rX = rY / rZ
		{"divw_rrc", { Opcode::DIV32_REG_REG_CONST}},   // rX = rY / const

		{"divl_rrr", { Opcode::DIV64_REG_REG_REG}},     // rX = rY / rZ
		{"divl_rrc", { Opcode::DIV64_REG_REG_CONST}},   // rX = rY / const

		{"jmp", { Opcode::JMP}},         // PC += CONST 

		{"restore_pc", { Opcode::RESTORE_PC}},     // PC = [rX + offset]
		{"move_pcrel", { Opcode::MOVE_PCREL_REG_CONST16}}
	};

	std::map<std::string, Register> regnames{
		{ "%r0", Register::R0 },
		{ "%r1", Register::R1 },
		{ "%r2", Register::R2 },
		{ "%r3", Register::R3 },
		{ "%r4", Register::R4 },
		{ "%r5", Register::R5 },
		{ "%r6", Register::R6 },
		{ "%r7", Register::R7 },
		{ "%r8", Register::R8 },
		{ "%sp", Register::SP },
		{ "%flags", Register::FLAGS },
	};

}
