#include "MachineInfo.h"

namespace ecc
{

	std::map<std::string, ExecuteStageInsnInfo> execInsnInfo{
		{ "nop", {ExecuteStageOpcode::EXEC_NOP}},
		{ "move_reg_value", {ExecuteStageOpcode::EXEC_MOVE_REG_VALUE}},
		{ "ORB_reg_value", {ExecuteStageOpcode::EXEC_ORB_REG_VALUE}},
		{ "ORC_reg_value", {ExecuteStageOpcode::EXEC_ORC_REG_VALUE}},

		{ "store", {ExecuteStageOpcode::EXEC_STORE_ADDR_VALUE}},
		{ "jmp", {ExecuteStageOpcode::EXEC_JMP}},
		{ "load_restore_pc", {ExecuteStageOpcode::EXEC_LOAD_RESTORE_PC}},
		{ "load_reg", {ExecuteStageOpcode::EXEC_LOAD_REG}},
		{ "cmp", {ExecuteStageOpcode::EXEC_CMP}},
		{ "cjmp", {ExecuteStageOpcode::EXEC_COND_JMP}},
		{ "add_reg_val_val", {ExecuteStageOpcode::EXEC_ADD_REG_VALUE_VALUE}},
		{ "shl_reg_val_val", {ExecuteStageOpcode::EXEC_SHL_REG_VALUE_VALUE}},
		{ "halt", {ExecuteStageOpcode::EXEC_HALT}},
	};


	std::map<std::string, InstructionInfo> insnInfo{
		{ "nop", { Opcode::NOP}},

		{ "move_LO", { Opcode::MOVE_R0_CONST24A }},
		{ "move_MID", { Opcode::MOVE_R0_CONST24B }},
		{ "move_HI", { Opcode::MOVE_R0_CONST24C }},

		{ "halt", { Opcode::HALT}},

		{"move_blockid", {Opcode::MOVE_REG_BLOCK_INDEX}},

		{"movreg2reg", { Opcode::MOVE_REG_REG}}, // rX = rY

		{"move_reg_cpuid", { Opcode::MOVE_REG_CPU_ID}},  // rX = <CPU_ID> (32 bit)

		{"move_reg_TS", { Opcode::MOVE_REG_TIMESTAMP}},  // rX = <TIMESTAMP> (64 bit)

		{"move_reg_c16", { Opcode::MOVE_REG_CONST16}},  // rX = CONST (16 bit)

		{"move_r0_ca", { Opcode::MOVE_R0_CONST24A}},  // r0  = CONST (24 bit)
		{"move_r0_cb", { Opcode::MOVE_R0_CONST24B}},   // r0 |= CONST << 24 bit
		{"move_r0_cc", { Opcode::MOVE_R0_CONST24C}},   // r0 |= CONST << 48 (64 - 48 = 16 bits)

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

	std::map<std::string, RegisterID> regnames{
		{ "%r0", RegisterID::REG_R0 },
		{ "%r1", RegisterID::REG_R1 },
		{ "%r2", RegisterID::REG_R2 },
		{ "%r3", RegisterID::REG_R3 },
		{ "%r4", RegisterID::REG_R4 },
		{ "%r5", RegisterID::REG_R5 },
		{ "%r6", RegisterID::REG_R6 },
		{ "%r7", RegisterID::REG_R7 },
		{ "%r8", RegisterID::REG_R8 },
		{ "%sp", RegisterID::REG_SP },
		{ "%flags", RegisterID::REG_FLAGS },
		{ "%pc", RegisterID::REG_PC },
		{ "%block_index", RegisterID::REG_BLOCK_INDEX },
	};

}
