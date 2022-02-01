
typedef longint unsigned uint64_t;
typedef longint signed int64_t;

typedef int unsigned uint32_t;
typedef int signed int32_t;

typedef shortint unsigned uint16_t;
typedef shortint signed int16_t;

typedef bit bool;

`include "global_decls.sv"

typedef uint64_t ExecutionMask;

typedef int64_t FetchToDecodeBus__Packet;
typedef int64_t InsnCacheMemoryBus__Packet;
typedef int64_t StoreToFetchBus__Packet;

`include "FetchStage.sv"

