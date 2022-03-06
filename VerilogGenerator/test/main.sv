
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


`define PACK8_BYTES(a0, a1, a2, a3, a4, a5, a6, a7) {>> {  a3, a2, a1, a0, a7, a6, a5, a4}}

`define PACK8_BITS(a0, a1, a2, a3, a4, a5, a6, a7) ( \
    ((uint64_t'(a0)) << 0) | \
	((uint64_t'(a1)) << 1) | \
	((uint64_t'(a2)) << 2) | \
	((uint64_t'(a3)) << 3) | \
	((uint64_t'(a4)) << 4) | \
	((uint64_t'(a5)) << 5) | \
	((uint64_t'(a6)) << 6) | \
	((uint64_t'(a7)) << 7))

`define PACK2(a0, a1) { a0, a1 }

`define COUNT_BITS64(x)   (uint32_t'($countones(x)))

`include "Defines.sv"



`include "VectorValue.sv"
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
end
endfunction;

module main(input clk);

    // bit clk;

    GlobalStats stats;

    DecodeToExecuteBus decode_exec_bus;
    FetchToDecodeBus fetch_decode_bus;
    StoreToFetchBus store_fetch_bus;
    ExecuteToStoreBus exec_store_bus;

    RegisterFile reg_file;
    DivergenceQueue divergence_queue;

    MemoryBus memory_bus;
    DRAM dram (memory_bus);

    MemoryBus core0_fetch_input;
    MemoryBus core0_vec_access_input;

    CoreInternalMemoryMultiplexer core0_fetch_store_multiplexer(memory_bus, core0_fetch_input, core0_vec_access_input);


    VecMemoryBus vec_mem_bus_to_cpu;
    VectorMemoryController#(.core_id(0)) vec_mem_controller(vec_mem_bus_to_cpu, core0_vec_access_input);

    FetchStage  #(.core_id(0)) fetcher (fetch_decode_bus, store_fetch_bus, core0_fetch_input, stats);
    DecodeStage #(.core_id(0)) decoder (fetch_decode_bus, decode_exec_bus, reg_file);
    ExecuteStage #(.core_id(0)) executor (decode_exec_bus, exec_store_bus, reg_file, stats);
    StoreStage #(.core_id(0)) storer (exec_store_bus, vec_mem_bus_to_cpu, reg_file, store_fetch_bus, divergence_queue, stats);





    initial begin
        int fd;
        uint64_t address;

        memory_bus.init();
        core0_fetch_input.init();
        core0_vec_access_input.init();

        stats.init();

        decode_exec_bus.init();
        fetch_decode_bus.init();
        store_fetch_bus.init();
        exec_store_bus.init();
        vec_mem_bus_to_cpu.init();

        divergence_queue.init();

        fd = $fopen("../../Assembler/tests/t1.bin", "rb");
        if (fd == 0) begin            
            $display("FAILED to open bin file");
            assert(0);
        end

        address = CODE_SEGMENT_START;
        while (! $feof(fd)) begin
            byte data;
            int status;
            status = $fread(data, fd);
            //$display ("CODE: status = %0d reg1 = %b",status,data);
            INITIAL_write_to_global_memory(address, data);
            address += 1;
        end
        $fclose(fd);

        fd = $fopen("../../Assembler/tests/t1.bin.data", "rb");
        if (fd == 0) begin
            $display("FAILED to open bin.data file");
            assert(0);
        end
        address = DATA_SEGMENT_START;
        while (! $feof(fd)) begin
            byte data;
            int status;
            status = $fread(data, fd);
            //$display ("DATA: status = %0d reg1 = %b",status,data);
            INITIAL_write_to_global_memory(address, data);
            address += 1;
        end
        $fclose(fd);
    end

     always @(posedge clk) 
     begin
        core0_fetch_store_multiplexer.run();
        fetcher.run();    
        decoder.run();    
        executor.run();
        storer.run();
     end 

     always @(posedge clk) 
     begin
        dram.run();    
     end 

endmodule;

