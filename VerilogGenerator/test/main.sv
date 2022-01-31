
typedef longint unsigned uint64_t;
typedef longint signed int64_t;

typedef int unsigned uint32_t;
typedef int signed int32_t;

typedef shortint unsigned uint16_t;
typedef shortint signed int16_t;

typedef bit bool;


typedef uint32_t instruction_t;
typedef uint64_t memory_address_t;
typedef uint64_t ExecutionMask;

typedef instruction_t fetched_instruction_data_t[2];

typedef int64_t FetchToDecodeBus__Packet;
typedef int64_t InsnCacheMemoryBus__Packet;
typedef int64_t StoreToFetchBus__Packet;

typedef byte unsigned Opcode;

`define INSTRUCTION_SIZE ($bits(instruction_t) / 8)

`include "FetchStage.sv"

