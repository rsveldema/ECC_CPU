
typedef longint unsigned uint64_t;
typedef longint signed int64_t;

typedef int unsigned uint32_t;
typedef int signed int32_t;

typedef shortint unsigned uint16_t;
typedef shortint signed int16_t;

typedef byte unsigned uint8_t;
typedef byte signed int8_t;

typedef bit bool;

typedef reg[20:0] phys_memory_address_t;


`include "Defines.sv"

`define PACK8(a0, a1, a2, a3, a4, a5, a6, a7) {>> { a0, a1,	a2, a3, a4, a5, a6, a7}}


typedef struct packed
{
		int64_t[8:0] data;
} VectorValue;

`include "Packets.sv"
//`include "MemoryBus.sv"
//`include "FetchStage.sv"

/*
function void CONTEXT_SWITCH();
begin
end
endfunction;
*/

function void abort();
begin
end
endfunction;

function void READ_MEMORY_DELAY();
begin
end
endfunction;

function void WRITE_MEMORY_DELAY();
begin
end
endfunction;

function void incFetchedInsnWait();
begin
    assert(0);
end
endfunction;

module main(input clk);

    // bit clk;

    FetchToDecodeBus decode_bus;
    StoreToFetchBus store_bus;
    MemoryBus memory_bus;
    DRAM dram (memory_bus);

    FetchStage  #(.core_id(0)) fetcher (decode_bus, store_bus, memory_bus);

    initial begin
        memory_bus.init();
        store_bus.init_store_to_fetch_bus();
    end

     always @(posedge clk) 
     begin
        fetcher.run();    
     end 

endmodule;

