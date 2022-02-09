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
		{ "nop", { Opcode::INSN_OPCODE_NOP}},

		{ "move_LO", { Opcode::INSN_OPCODE_MOVE_R0_CONST24A }},
		{ "move_MID", { Opcode::INSN_OPCODE_MOVE_R0_CONST24B }},
		{ "move_HI", { Opcode::INSN_OPCODE_MOVE_R0_CONST24C }},

		{ "halt", { Opcode::INSN_OPCODE_HALT}},

		{"move_blockid", {Opcode::INSN_OPCODE_MOVE_REG_BLOCK_INDEX}},

		{"movreg2reg", { Opcode::INSN_OPCODE_MOVE_REG_REG}}, // rX = rY

		{"move_reg_cpuid", { Opcode::INSN_OPCODE_MOVE_REG_CPU_ID}},  // rX = <CPU_ID> (32 bit)

		{"move_reg_TS", { Opcode::INSN_OPCODE_MOVE_REG_TIMESTAMP}},  // rX = <TIMESTAMP> (64 bit)

		{"move_reg_c16", { Opcode::INSN_OPCODE_MOVE_REG_CONST16}},  // rX = CONST (16 bit)

		{"move_r0_ca", { Opcode::INSN_OPCODE_MOVE_R0_CONST24A}},  // r0  = CONST (24 bit)
		{"move_r0_cb", { Opcode::INSN_OPCODE_MOVE_R0_CONST24B}},   // r0 |= CONST << 24 bit
		{"move_r0_cc", { Opcode::INSN_OPCODE_MOVE_R0_CONST24C}},   // r0 |= CONST << 48 (64 - 48 = 16 bits)

		{"load_rcr", { Opcode::INSN_OPCODE_LOAD_REG_CONST_REG}},        // rX = [rY + offset] 
		{"store_rcr", { Opcode::INSN_OPCODE_STORE_REG_CONST_REG}}, // [rY + offset] = rX

		{"cmp_reg_reg", { Opcode::INSN_OPCODE_CMP_REG_REG}},        // flags = rX <> rY

		{"add_rrr", { Opcode::INSN_OPCODE_ADD_REG_REG_REG}},     // rX = rY + rZ
		{"add_rrc", { Opcode::INSN_OPCODE_ADD_REG_REG_CONST}},   // rX = rY + const

		{"rs_rrr", { Opcode::INSN_OPCODE_RSHIFT_REG_REG_REG}},     // rX = rY << rZ
		{"rs_rrc", { Opcode::INSN_OPCODE_RSHIFT_REG_REG_CONST}},   // rX = rY << const

		{"ls_rrr", { Opcode::INSN_OPCODE_L_SSHIFT_REG_REG_REG}},     // rX = rY >>> rZ
		{"ls_rrc", { Opcode::INSN_OPCODE_L_SSHIFT_REG_REG_CONST}},   // rX = rY >>> const

		{"uls_rrr", { Opcode::INSN_OPCODE_L_USHIFT_REG_REG_REG}},     // rX = rY >> rZ
		{"uls_rrc", { Opcode::INSN_OPCODE_L_USHIFT_REG_REG_CONST}},   // rX = rY >> const

		{"mul_rrr", { Opcode::INSN_OPCODE_MUL_REG_REG_REG}},     // rX = rY * rZ
		{"mul_rrc", { Opcode::INSN_OPCODE_MUL_REG_REG_CONST}},   // rX = rY * const

		{"div_rrr", { Opcode::INSN_OPCODE_DIV_REG_REG_REG}},     // rX = rY / rZ
		{"div_rrc", { Opcode::INSN_OPCODE_DIV_REG_REG_CONST}},   // rX = rY / const

		{"jmp", { Opcode::INSN_OPCODE_JMP_ALWAYS}},         // PC += CONST 
		{"je", { Opcode::INSN_OPCODE_JMP_EQUAL}},         // PC += CONST 
		{"jne", { Opcode::INSN_OPCODE_JMP_NOT_EQUAL}},         // PC += CONST 
		{"jl", { Opcode::INSN_OPCODE_JMP_LOWER}},         // PC += CONST 
		{"jle", { Opcode::INSN_OPCODE_JMP_LOWER_EQUAL}},         // PC += CONST 
		{"jg", { Opcode::INSN_OPCODE_JMP_GREATER}},         // PC += CONST 
		{"jge", { Opcode::INSN_OPCODE_JMP_GREATER_EQUAL}},         // PC += CONST 

		{"restore_pc", { Opcode::INSN_OPCODE_LOAD_RESTORE_PC}},     // PC = [rX + offset]
		{"move_pcrel", { Opcode::INSN_OPCODE_MOVE_PCREL_REG_CONST16}}
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
