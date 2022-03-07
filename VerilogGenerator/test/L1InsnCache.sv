

typedef struct packed {
	memory_address_t address;
	int64_t payload;
} L1InsnCacheLine;

parameter uint32_t NUM_CACHE_LINES_L1_INSN_CACHE = 128;


function uint32_t get_l1_insn_cache_index(input memory_address_t address);
begin
begin
	assert(((address & (8 - 1)) == 0));
	return (((uint32_t'(address)) >> 3) & (NUM_CACHE_LINES_L1_INSN_CACHE - 1));
end
end
endfunction


module L1InsnCache(MemoryBus toCPU, MemoryBus toMemory, GlobalStats stats);
	parameter CoreID core_id;
	reg[32:0] state = 0;
	L1InsnCacheLine[(NUM_CACHE_LINES_L1_INSN_CACHE) - 1:0] L1InsnCacheLines;
	BusPacket cpu_pkt;
	BusPacket response_pkt;



	
	
	task run();
	case (state)
		0:
			begin
				for (uint32_t i = 0; (i < NUM_CACHE_LINES_L1_INSN_CACHE); i=(i + 1))
					begin
						L1InsnCacheLines[i].address <= 100000000;
					end
				// CONTEXT_SWITCH();
				state <= 269; // GOTO
				return;
			end
		269:
			begin
				state <= 270; // GOTO
				return;
			end
		270:
			begin
				if (!(toCPU.request_busy))
				begin
					state <= 272; // GOTO
					return;
				end
				cpu_pkt <= toCPU.accept_request();
				// CONTEXT_SWITCH();
				state <= 273; // GOTO
				return;
			end
		273:
			begin
				if ((L1InsnCacheLines[get_l1_insn_cache_index(cpu_pkt.address)].address == cpu_pkt.address))
				begin
				end
				else
				begin
					state <= 278; // GOTO
					return;
				end
				stats.incnumL1InsnCacheHits();
				state <= 275; // GOTO
				return;
			end
		275:
			begin
				// CONTEXT_SWITCH();
				state <= 277; // GOTO
				return;
			end
		277:
			begin
				if (toCPU.response_busy)
				begin
					state <= 275; // GOTO
					return;
				end
				state <= 276; // GOTO
				return;
			end
		276:
			begin
				toCPU.send_read_response(L1InsnCacheLines[get_l1_insn_cache_index(cpu_pkt.address)].payload, cpu_pkt.source, cpu_pkt.address);
				state <= 274; // GOTO
				return;
			end
		278:
			begin
				state <= 279; // GOTO
				return;
			end
		279:
			begin
				// CONTEXT_SWITCH();
				state <= 281; // GOTO
				return;
			end
		281:
			begin
				if (toMemory.request_busy)
				begin
					state <= 279; // GOTO
					return;
				end
				state <= 280; // GOTO
				return;
			end
		280:
			begin
				toMemory.send_request(cpu_pkt);
				state <= 274; // GOTO
				return;
			end
		274:
			begin
				state <= 272; // GOTO
				return;
			end
		272:
			begin
				if (!(toMemory.response_busy))
				begin
					state <= 282; // GOTO
					return;
				end
				response_pkt <= toMemory.get_response();
				// CONTEXT_SWITCH();
				state <= 283; // GOTO
				return;
			end
		283:
			begin
				$display("storing into cache: address: ", (uint32_t'(response_pkt.address)), get_l1_insn_cache_index(response_pkt.address), (uint32_t'(response_pkt.payload)));
				L1InsnCacheLines[get_l1_insn_cache_index(response_pkt.address)] <= {response_pkt.address, (int64_t'(response_pkt.payload))};
				state <= 284; // GOTO
				return;
			end
		284:
			begin
				// CONTEXT_SWITCH();
				state <= 286; // GOTO
				return;
			end
		286:
			begin
				if (toCPU.response_busy)
				begin
					state <= 284; // GOTO
					return;
				end
				state <= 285; // GOTO
				return;
			end
		285:
			begin
				toCPU.send_response(response_pkt);
				state <= 282; // GOTO
				return;
			end
		282:
			begin
				// CONTEXT_SWITCH();
				state <= 287; // GOTO
				return;
			end
		287:
			begin
				state <= 270; // GOTO
				return;
			end
		271:
			begin
			end
	endcase
	endtask
endmodule
