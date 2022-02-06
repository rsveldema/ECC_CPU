#pragma once

#define INTERFACE struct

namespace ecc
{
	using instruction_t = uint32_t;

	using memory_address_t = uint64_t;

	using fetched_instruction_data_t = std::array<instruction_t, 2>;

	using execution_mask_t = uint64_t;

	static constexpr unsigned POINTER_SIZE = sizeof(memory_address_t);
	static constexpr auto INSTRUCTION_SIZE = sizeof(instruction_t);

	static constexpr memory_address_t CODE_SEGMENT_START = 0;
	static constexpr memory_address_t DATA_SEGMENT_START = 1024 * 1024;

	static constexpr unsigned VECTOR_MEM_SIZE = 64;

	static constexpr unsigned NUMBER_OF_VECTOR_THREADS_INT64 = (VECTOR_MEM_SIZE / sizeof(uint64_t));

	static constexpr uint64_t ALL_THREADS_EXEC_MASK_INT64 = (1 << NUMBER_OF_VECTOR_THREADS_INT64) - 1;


	enum class RegisterID : uint8_t
	{
		REG_R0,
		REG_R1,
		REG_R2,
		REG_R3,

		REG_R4,
		REG_R5,
		REG_R6,
		REG_R7,

		REG_R8,
		REG_R9,

		REG_BLOCK_INDEX,
		REG_SP,
		REG_FLAGS,
		REG_PC,

		MAX_REG_ID
	};

	static constexpr uint64_t MACHINE_FLAGS_MASK_HALT = (1 << 0);

	static constexpr uint64_t FLAGS_MASK_EQ = (1 << 1);
	static constexpr uint64_t FLAGS_MASK_GT = (1 << 2);
	static constexpr uint64_t FLAGS_MASK_LT = (1 << 3);

	enum class CoreID : uint8_t
	{
		Core0,
		Core1,
		Core2,
		Core3,
		Core4,
		Core5,
		Core6,
		Core7
	};

	enum class CoreComponentID : uint8_t
	{
		FETCH,
		DECODE,
		EXECUTE,
		STORE
	};


	enum class Opcode : uint8_t
	{
		NOP,
		HALT,

		MOVE_REG_BLOCK_INDEX,

		MOVE_REG_REG, // rX = rY

		MOVE_REG_CPU_ID,  // rX = <CPU_ID> (32 bit)

		MOVE_REG_TIMESTAMP,  // rX = <TIMESTAMP> (64 bit)

		MOVE_REG_CONST16,  // rX = CONST (16 bit)

		MOVE_R0_CONST24A,  // r0  = CONST (24 bit)
		MOVE_R0_CONST24B,   // r0 |= CONST << 24 bit
		MOVE_R0_CONST24C,   // r0 |= CONST << 48 (64 - 48 = 16 bits)

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
		MOVE_PCREL_REG_CONST16
	};

	enum class ExecuteStageOpcode : uint8_t
	{
		EXEC_NOP,
		EXEC_MOVE_REG_VALUE,
		EXEC_STORE_ADDR_VALUE,
		EXEC_JMP,
		EXEC_LOAD_RESTORE_PC,
		EXEC_LOAD_REG,
		EXEC_CMP,
		EXEC_COND_JMP,
		EXEC_ADD_REG_VALUE_VALUE,
		EXEC_SHL_REG_VALUE_VALUE,

		EXEC_ORB_REG_VALUE,
		EXEC_ORC_REG_VALUE,

		EXEC_HALT
	};

	enum class StorageStageOpcode : uint8_t
	{
		STORAGE_NOP,
		STORAGE_STORE_VALUE_INTO_REG,
		STORAGE_STORE_REG_INTO_MEM,
		STORAGE_JMP,
		STORAGE_LOAD_MEM_INTO_REG,
		STORAGE_CJMP,
		STORAGE_HALT
	};

	struct BusID
	{
		CoreID core_id;
		CoreComponentID within_core_id;
	};

	static 
	uint32_t count_num_bits64(uint64_t value)
	{
		uint32_t c = 0;
		uint64_t f = value;
		for (unsigned i = 0; i < 64; i++)
		{
			c += (f & 1);

			f = f >> 1;
		}
		return c;
	}
}

