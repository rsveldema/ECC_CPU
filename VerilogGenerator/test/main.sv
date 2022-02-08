
typedef longint unsigned uint64_t;
typedef longint signed int64_t;

typedef int unsigned uint32_t;
typedef int signed int32_t;

typedef shortint unsigned uint16_t;
typedef shortint signed int16_t;

typedef bit bool;

`include "Defines.sv"
//`include "Packets.sv"
//`include "MemoryBus.sv"
//`include "FetchStage.sv"

module main();

    FetchToDecodeBus decode_bus;
    StoreToFetchBus store_bus;
    MemoryBus memory_bus;

    //FetchStage fetcher;

    initial begin
    //    memory_bus.init_memory_bus();
     //   init_store_to_fetch_bus();
    end

endmodule;

