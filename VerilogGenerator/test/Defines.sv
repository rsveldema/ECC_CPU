
typedef uint32_t instruction_t;

typedef uint64_t memory_address_t;

typedef instruction_t[(2) - 1:0] fetched_instruction_data_t;

typedef uint64_t execution_mask_t;

parameter POINTER_SIZE =  ((uint64_t'($bits(memory_address_t)) >> 3)  ) ;

parameter INSTRUCTION_SIZE =  ((uint64_t'($bits(instruction_t)) >> 3)  ) ;

parameter CODE_SEGMENT_START = 0;

parameter DATA_SEGMENT_START = (1024 * 1024);

parameter VECTOR_MEM_SIZE = 64;

parameter NUMBER_OF_VECTOR_THREADS_INT64 = (VECTOR_MEM_SIZE /  ((uint64_t'($bits(uint64_t)) >> 3)  ) );

parameter ALL_THREADS_EXEC_MASK_INT64 = ((1 << NUMBER_OF_VECTOR_THREADS_INT64) - 1);


typedef enum {
	REG_R0
	,REG_R1
	,REG_R2
	,REG_R3
	,REG_R4
	,REG_R5
	,REG_R6
	,REG_R7
	,REG_R8
	,REG_R9
	,REG_BLOCK_INDEX
	,REG_SP
	,REG_FLAGS
	,REG_PC
	,MAX_REG_ID
} RegisterID;

parameter MACHINE_FLAGS_MASK_HALT = (1 << 0);

parameter FLAGS_MASK_EQ = (1 << 1);

parameter FLAGS_MASK_GT = (1 << 2);

parameter FLAGS_MASK_LT = (1 << 3);


typedef enum {
	CORE_ID_Core0
	,CORE_ID_Core1
	,CORE_ID_Core2
	,CORE_ID_Core3
	,CORE_ID_Core4
	,CORE_ID_Core5
	,CORE_ID_Core6
	,CORE_ID_Core7
} CoreID;


typedef enum {
	COMPONENT_TYPE_FETCH
	,COMPONENT_TYPE_DECODE
	,COMPONENT_TYPE_EXECUTE
	,COMPONENT_TYPE_STORE
} CoreComponentID;


typedef enum {
	INSN_OPCODE_NOP
	,INSN_OPCODE_HALT
	,INSN_OPCODE_MOVE_REG_BLOCK_INDEX
	,INSN_OPCODE_MOVE_REG_REG
	,INSN_OPCODE_MOVE_REG_CPU_ID
	,INSN_OPCODE_MOVE_REG_TIMESTAMP
	,INSN_OPCODE_MOVE_REG_CONST16
	,INSN_OPCODE_MOVE_R0_CONST24A
	,INSN_OPCODE_MOVE_R0_CONST24B
	,INSN_OPCODE_MOVE_R0_CONST24C
	,INSN_OPCODE_LOAD_REG_CONST_REG
	,INSN_OPCODE_STORE_REG_CONST_REG
	,INSN_OPCODE_CMP_REG_REG
	,INSN_OPCODE_ADD_REG_REG_REG
	,INSN_OPCODE_ADD_REG_REG_CONST
	,INSN_OPCODE_RSHIFT_REG_REG_REG
	,INSN_OPCODE_RSHIFT_REG_REG_CONST
	,INSN_OPCODE_L_SSHIFT_REG_REG_REG
	,INSN_OPCODE_L_SSHIFT_REG_REG_CONST
	,INSN_OPCODE_L_USHIFT_REG_REG_REG
	,INSN_OPCODE_L_USHIFT_REG_REG_CONST
	,INSN_OPCODE_MUL_REG_REG_REG
	,INSN_OPCODE_MUL_REG_REG_CONST
	,INSN_OPCODE_DIV_REG_REG_REG
	,INSN_OPCODE_DIV_REG_REG_CONST
	,INSN_OPCODE_JMP_ALWAYS
	,INSN_OPCODE_JMP_EQUAL
	,INSN_OPCODE_JMP_NOT_EQUAL
	,INSN_OPCODE_JMP_LOWER
	,INSN_OPCODE_JMP_LOWER_EQUAL
	,INSN_OPCODE_JMP_GREATER
	,INSN_OPCODE_JMP_GREATER_EQUAL
	,INSN_OPCODE_LOAD_RESTORE_PC
	,INSN_OPCODE_MOVE_PCREL_REG_CONST16
} Opcode;


typedef enum {
	EXEC_NOP
	,EXEC_MOVE_REG_VALUE
	,EXEC_STORE_ADDR_VALUE
	,EXEC_JMP
	,EXEC_LOAD_RESTORE_PC
	,EXEC_LOAD_REG
	,EXEC_CMP
	,EXEC_COND_JMP
	,EXEC_ADD_REG_VALUE_VALUE
	,EXEC_SHL_REG_VALUE_VALUE
	,EXEC_ORB_REG_VALUE
	,EXEC_ORC_REG_VALUE
	,EXEC_HALT
} ExecuteStageOpcode;


typedef enum {
	STORAGE_NOP
	,STORAGE_STORE_VALUE_INTO_REG
	,STORAGE_STORE_REG_INTO_MEM
	,STORAGE_JMP
	,STORAGE_LOAD_MEM_INTO_REG
	,STORAGE_CJMP
	,STORAGE_HALT
} StorageStageOpcode;


typedef struct packed {
	CoreID core_id;
	CoreComponentID within_core_id;
} BusID;


function Opcode getOpcode(input instruction_t insn);
begin
begin
	return (Opcode'((insn & 32'hff)));
end
end
endfunction


function BusID createBusID(input CoreID core_id, input CoreComponentID within_id);
begin
	BusID ret;
begin
	ret.core_id <= core_id;
	ret.within_core_id <= within_id;
	return ret;
end
end
endfunction


function uint32_t count_num_bits64(input uint64_t value);
begin
	uint32_t c;
	uint64_t f;
begin
	c <= 0;
	f <= value;
	for (uint32_t i = 0; (i < 64); i=(i + 1))
		begin
			c <= c + (uint32_t'((f & 1)));
			f <= (f >> 1);
		end
	return c;
end
end
endfunction
