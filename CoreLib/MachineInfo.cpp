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

		{"load_rcr", { Opcode::LOAD_REG_CONST_REG}},        // rX = [rY + offset] 
		{"store_rcr", { Opcode::STORE_REG_CONST_REG}}, // [rY + offset] = rX

		{"cmp_reg_reg", { Opcode::CMP_REG_REG}},        // flags = rX <> rY

		{"add_rrr", { Opcode::ADD_REG_REG_REG}},     // rX = rY + rZ
		{"add_rrc", { Opcode::ADD_REG_REG_CONST}},   // rX = rY + const

		{"rs_rrr", { Opcode::RSHIFT_REG_REG_REG}},     // rX = rY << rZ
		{"rs_rrc", { Opcode::RSHIFT_REG_REG_CONST}},   // rX = rY << const

		{"ls_rrr", { Opcode::L_SSHIFT_REG_REG_REG}},     // rX = rY >>> rZ
		{"ls_rrc", { Opcode::L_SSHIFT_REG_REG_CONST}},   // rX = rY >>> const

		{"uls_rrr", { Opcode::L_USHIFT_REG_REG_REG}},     // rX = rY >> rZ
		{"uls_rrc", { Opcode::L_USHIFT_REG_REG_CONST}},   // rX = rY >> const

		{"mul_rrr", { Opcode::MUL_REG_REG_REG}},     // rX = rY * rZ
		{"mul_rrc", { Opcode::MUL_REG_REG_CONST}},   // rX = rY * const

		{"div_rrr", { Opcode::DIV_REG_REG_REG}},     // rX = rY / rZ
		{"div_rrc", { Opcode::DIV_REG_REG_CONST}},   // rX = rY / const

		{"jmp", { Opcode::JMP_ALWAYS}},         // PC += CONST 
		{"je", { Opcode::JMP_EQUAL}},         // PC += CONST 
		{"jne", { Opcode::JMP_NOT_EQUAL}},         // PC += CONST 
		{"jl", { Opcode::JMP_LOWER}},         // PC += CONST 
		{"jle", { Opcode::JMP_LOWER_EQUAL}},         // PC += CONST 
		{"jg", { Opcode::JMP_GREATER}},         // PC += CONST 
		{"jge", { Opcode::JMP_GREATER_EQUAL}},         // PC += CONST 

		{"restore_pc", { Opcode::LOAD_RESTORE_PC}},     // PC = [rX + offset]
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
		{ "%pc", Register::PC },
	};

}
