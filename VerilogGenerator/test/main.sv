
typedef longint unsigned uint64_t;
typedef longint signed int64_t;

typedef int unsigned uint32_t;
typedef int signed int32_t;

typedef shortint unsigned uint16_t;
typedef shortint signed int16_t;

typedef bit bool;

`include "Defines.sv"
`include "Packets.sv"
//`include "MemoryBus.sv"
//`include "FetchStage.sv"

function CONTEXT_SWITCH();
begin
end
endfunction;

function abort();
begin
    assert(0);
end
endfunction;

function incFetchedInsnWait();
begin
    assert(0);
end
endfunction;

function error(msg);
begin
    $display(msg);
    $finish();
end
endfunction;

module main();

    // bit clk;

    FetchToDecodeBus decode_bus;
    StoreToFetchBus store_bus;
    MemoryBus memory_bus;
    BusID fetcher_bus_id = createBusID(0, 
                COMPONENT_TYPE_FETCH);

    FetchStage fetcher(decode_bus, store_bus, memory_bus, fetcher_bus_id);

    initial begin
        memory_bus.init();
        store_bus.init_store_to_fetch_bus();

   end

endmodule;

