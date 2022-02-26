#pragma once

#include <array>

#define INTERFACE struct
#define METHOD_SECTION

namespace ecc
{
	using instruction_t = uint32_t;
	using flags_reg_t = uint64_t;

	using memory_address_t = uint64_t;

	using execution_mask_t = uint64_t;

	// fits a int64_t at the moment which we use to simplify memory bus access.
	using fetched_instruction_data_t = std::array<instruction_t, 2>;

	static constexpr uint32_t DRAM_READ_ACCESS_CYCLES = 100;
	static constexpr uint32_t DRAM_WRITE_ACCESS_CYCLES = 100;

	static constexpr uint32_t POINTER_SIZE = static_cast<uint32_t>(sizeof(memory_address_t));
	static constexpr uint32_t INSTRUCTION_SIZE = static_cast<uint32_t>(sizeof(instruction_t));

	static constexpr memory_address_t CODE_SEGMENT_START = 0;
	static constexpr memory_address_t DATA_SEGMENT_START = 1024 * 1024;
	static constexpr memory_address_t DATA_SEGMENT_SIZE  = 1024 * 1024;
	static constexpr memory_address_t END_MEMORY_ADDRESS = DATA_SEGMENT_START + DATA_SEGMENT_SIZE;

	static constexpr uint32_t VECTOR_MEM_SIZE = 64;

	static constexpr uint32_t NUMBER_OF_VECTOR_THREADS_INT64 = (VECTOR_MEM_SIZE / static_cast<uint32_t>(sizeof(uint64_t)));

	static constexpr uint64_t ALL_THREADS_EXEC_MASK_INT64 = (1 << NUMBER_OF_VECTOR_THREADS_INT64) - 1;


	enum RegisterID : uint8_t
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

	static constexpr flags_reg_t MACHINE_FLAGS_MASK_HALT = (1 << 0);

	static constexpr flags_reg_t FLAGS_MASK_EQ = (1 << 1);
	static constexpr flags_reg_t FLAGS_MASK_GT = (1 << 2);
	static constexpr flags_reg_t FLAGS_MASK_LT = (1 << 3);

	enum CoreID : uint8_t
	{
		CORE_ID_Core0,
		CORE_ID_Core1,
		CORE_ID_Core2,
		CORE_ID_Core3,
		CORE_ID_Core4,
		CORE_ID_Core5,
		CORE_ID_Core6,
		CORE_ID_Core7
	};

	enum CoreComponentID : uint8_t
	{
		COMPONENT_TYPE_FETCH,
		COMPONENT_TYPE_DECODE,
		COMPONENT_TYPE_EXECUTE,
		COMPONENT_TYPE_STORE
	};


	enum Opcode : uint8_t
	{
		INSN_OPCODE_NOP,
		INSN_OPCODE_HALT,

		INSN_OPCODE_MOVE_REG_BLOCK_INDEX,

		INSN_OPCODE_MOVE_REG_REG, // rX = rY

		INSN_OPCODE_MOVE_REG_CPU_ID,  // rX = <CPU_ID> (32 bit)

		INSN_OPCODE_MOVE_REG_TIMESTAMP,  // rX = <TIMESTAMP> (64 bit)

		INSN_OPCODE_MOVE_REG_CONST16,  // rX = CONST (16 bit)

		INSN_OPCODE_MOVE_R0_CONST24A,  // r0  = CONST (24 bit)
		INSN_OPCODE_MOVE_R0_CONST24B,   // r0 |= CONST << 24 bit
		INSN_OPCODE_MOVE_R0_CONST24C,   // r0 |= CONST << 48 (64 - 48 = 16 bits)

		INSN_OPCODE_LOAD_REG_CONST_REG,       // rX = [rY + offset] 
		INSN_OPCODE_STORE_REG_CONST_REG,  // [rY + offset] = rX

		INSN_OPCODE_CMP_REG_REG,        // flags = rX <> rY

		INSN_OPCODE_ADD_REG_REG_REG,     // rX = rY + rZ
		INSN_OPCODE_ADD_REG_REG_CONST,   // rX = rY + const

		INSN_OPCODE_RSHIFT_REG_REG_REG,     // rX = rY << rZ
		INSN_OPCODE_RSHIFT_REG_REG_CONST,   // rX = rY << const

		INSN_OPCODE_L_SSHIFT_REG_REG_REG,     // rX = rY >>> rZ
		INSN_OPCODE_L_SSHIFT_REG_REG_CONST,   // rX = rY >>> const

		INSN_OPCODE_L_USHIFT_REG_REG_REG,     // rX = rY >> rZ
		INSN_OPCODE_L_USHIFT_REG_REG_CONST,   // rX = rY >> const

		INSN_OPCODE_MUL_REG_REG_REG,     // rX = rY * rZ
		INSN_OPCODE_MUL_REG_REG_CONST,   // rX = rY * const

		INSN_OPCODE_DIV_REG_REG_REG,     // rX = rY / rZ
		INSN_OPCODE_DIV_REG_REG_CONST,   // rX = rY / const

		INSN_OPCODE_JMP_ALWAYS,         // PC += CONST 
		INSN_OPCODE_JMP_EQUAL,
		INSN_OPCODE_JMP_NOT_EQUAL,
		INSN_OPCODE_JMP_LOWER,
		INSN_OPCODE_JMP_LOWER_EQUAL,
		INSN_OPCODE_JMP_GREATER,
		INSN_OPCODE_JMP_GREATER_EQUAL,

		INSN_OPCODE_LOAD_RESTORE_PC,     // PC = [rX + offset]
		INSN_OPCODE_MOVE_PCREL_REG_CONST16
	};

	enum ExecuteStageOpcode : uint8_t
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

	enum StorageStageOpcode : uint8_t
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

	METHOD_SECTION;

	static 
	inline bool isValidIndex(const RegisterID id)
	{
		return
			(id >= REG_R0) &
			(id < MAX_REG_ID);
	}


	static
	inline Opcode getOpcode(instruction_t insn)
	{
		return static_cast<Opcode>(insn);
	}


	static inline
	BusID createBusID(CoreID core_id, CoreComponentID within_id)
	{
		BusID ret;
		ret.core_id = core_id;
		ret.within_core_id = within_id;
		return ret;
	}

	static inline
	uint32_t count_num_bits64(const execution_mask_t& value)
	{
		uint32_t c = 0;
		for (uint32_t i = 0; i < NUMBER_OF_VECTOR_THREADS_INT64; i++)
		{
			c += static_cast<uint32_t>((value & (1 << i)) != 0);
		}
		return c;
	}
}

