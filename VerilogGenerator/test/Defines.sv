
typedef uint32_t instruction_t;

typedef uint64_t memory_address_t;

typedef instruction_t[(2) - 1:0] fetched_instruction_data_t;

typedef uint64_t execution_mask_t;

`define POINTER_SIZE ($bits(memory_address_t) / 8)

`define INSTRUCTION_SIZE ($bits(instruction_t) / 8)

`define CODE_SEGMENT_START 0

`define DATA_SEGMENT_START (1024 * 1024)

`define VECTOR_MEM_SIZE 64

`define NUMBER_OF_VECTOR_THREADS_INT64 (VECTOR_MEM_SIZE / ($bits(uint64_t) / 8))

`define ALL_THREADS_EXEC_MASK_INT64 ((1 << NUMBER_OF_VECTOR_THREADS_INT64) - 1)


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

`define MACHINE_FLAGS_MASK_HALT (1 << 0)

`define FLAGS_MASK_EQ (1 << 1)

`define FLAGS_MASK_GT (1 << 2)

`define FLAGS_MASK_LT (1 << 3)


typedef enum {
	Core0
	,Core1
	,Core2
	,Core3
	,Core4
	,Core5
	,Core6
	,Core7
} CoreID;


typedef enum {
	FETCH
	,DECODE
	,EXECUTE
	,STORE
} CoreComponentID;


typedef enum {
	NOP
	,HALT
	,MOVE_REG_BLOCK_INDEX
	,MOVE_REG_REG
	,MOVE_REG_CPU_ID
	,MOVE_REG_TIMESTAMP
	,MOVE_REG_CONST16
	,MOVE_R0_CONST24A
	,MOVE_R0_CONST24B
	,MOVE_R0_CONST24C
	,LOAD_REG_CONST_REG
	,STORE_REG_CONST_REG
	,CMP_REG_REG
	,ADD_REG_REG_REG
	,ADD_REG_REG_CONST
	,RSHIFT_REG_REG_REG
	,RSHIFT_REG_REG_CONST
	,L_SSHIFT_REG_REG_REG
	,L_SSHIFT_REG_REG_CONST
	,L_USHIFT_REG_REG_REG
	,L_USHIFT_REG_REG_CONST
	,MUL_REG_REG_REG
	,MUL_REG_REG_CONST
	,DIV_REG_REG_REG
	,DIV_REG_REG_CONST
	,JMP_ALWAYS
	,JMP_EQUAL
	,JMP_NOT_EQUAL
	,JMP_LOWER
	,JMP_LOWER_EQUAL
	,JMP_GREATER
	,JMP_GREATER_EQUAL
	,LOAD_RESTORE_PC
	,MOVE_PCREL_REG_CONST16
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


function uint32_t count_num_bits64(uint64_t value);
	uint32_t c;
	uint64_t f;
begin
	c = 0;
	f = value;
	for (uint32_t i = 0; (i < 64); i=(i + 1))
		begin
			c+=(uint32_t'((f & 1)));
			f=(f >> 1);
		end
	return c;
end
endfunction
