

typedef union packed {
	uint64_t value;
	fetched_instruction_data_t data;
} int64_to_insn_data;


function fetched_instruction_data_t getInsnData(input uint64_t value);
begin
	int64_to_insn_data tmp;
begin
	tmp.value <= value;
	return tmp.data;
end
end
endfunction


function bool changesControlFlow(input Opcode op);
begin
begin
	case (op)
		INSN_OPCODE_HALT,
		INSN_OPCODE_JMP_ALWAYS,
		INSN_OPCODE_JMP_EQUAL,
		INSN_OPCODE_JMP_NOT_EQUAL,
		INSN_OPCODE_JMP_GREATER,
		INSN_OPCODE_JMP_GREATER_EQUAL,
		INSN_OPCODE_JMP_LOWER,
		INSN_OPCODE_JMP_LOWER_EQUAL,
		INSN_OPCODE_LOAD_RESTORE_PC:
			begin
				return 1;
			end
		default:
			begin
				return 0;
			end
	endcase
	return 0;
end
end
endfunction


module FetchStage(FetchToDecodeBus decode_bus, StoreToFetchBus store_bus, MemoryBus memory_bus);
	parameter CoreID core_id;
	reg[32:0] state = 0;
	bool have_outstanding_jmp;
	memory_address_t fetch_PC;
	execution_mask_t exec_mask;
	memory_address_t address_cached;
	fetched_instruction_data_t insn_data_cached;
	StoreToFetchPacket jmp_retarget;
	memory_address_t address_fetched;
	BusPacket response;
	instruction_t insn;
	memory_address_t old_PC;
	FetchToDecodeBusPacket pkt;



	
	
	task run();
	case (state)
		0:
			begin
				have_outstanding_jmp <= 0;
				fetch_PC <= 0;
				exec_mask <= ALL_THREADS_EXEC_MASK_INT64;
				address_cached <= 64'hffffffffffffffff;
				// CONTEXT_SWITCH();
				state <= 1; // GOTO
				return;
			end
		1:
			begin
				$display("have outstanding jmp: ", have_outstanding_jmp);
				if (!(have_outstanding_jmp))
				begin
					state <= 3; // GOTO
					return;
				end
				have_outstanding_jmp <= 0;
				$display("waiting for store pipline to tell us the cond-jump-address");
				state <= 4; // GOTO
				return;
			end
		4:
			begin
				if (!(store_bus.can_receive))
				begin
					state <= 6; // GOTO
					return;
				end
				jmp_retarget <= store_bus.recv();
				// CONTEXT_SWITCH();
				fetch_PC <= jmp_retarget.newpc;
				exec_mask <= jmp_retarget.exec_mask;
				// CONTEXT_SWITCH();
				state <= 5; // GOTO
				return;
			end
		6:
			begin
				// CONTEXT_SWITCH();
				state <= 4; // GOTO
				return;
			end
		5:
			begin
				state <= 3; // GOTO
				return;
			end
		3:
			begin
				$display("testing cache adress");
				if ((address_cached == fetch_PC))
				begin
				end
				else
				begin
					state <= 8; // GOTO
					return;
				end
				$display("testing cache adress: already have it cached 1");
				state <= 7; // GOTO
				return;
			end
		8:
			begin
				if (((address_cached +  ((uint64_t'($bits(instruction_t)) >> 3)  ) ) == fetch_PC))
				begin
				end
				else
				begin
					state <= 10; // GOTO
					return;
				end
				$display("testing cache adress: already have it cached 2");
				state <= 9; // GOTO
				return;
			end
		10:
			begin
				address_fetched <= (fetch_PC & ~(7));
				$display("requesting memory at address: ", address_fetched);
				memory_bus.send_read_request_data(address_fetched, createBusID(core_id, COMPONENT_TYPE_FETCH));
				state <= 11; // GOTO
				return;
			end
		11:
			begin
				if (!(memory_bus.response_busy))
				begin
					state <= 13; // GOTO
					return;
				end
				response <= memory_bus.get_response();
				$display("response received from caches: ", response.packet_type);
				// CONTEXT_SWITCH();
				assert((response.packet_type == bus_read_response));
				address_cached <= address_fetched;
				insn_data_cached <= getInsnData(response.payload);
				// CONTEXT_SWITCH();
				state <= 12; // GOTO
				return;
			end
		13:
			begin
				incFetchedInsnWait();
				// CONTEXT_SWITCH();
				state <= 11; // GOTO
				return;
			end
		12:
			begin
				state <= 9; // GOTO
				return;
			end
		9:
			begin
				state <= 7; // GOTO
				return;
			end
		7:
			begin
				$display("testing cache adress: retrieving from local cache");
				insn <= 0;
				if ((address_cached == fetch_PC))
				begin
				end
				else
				begin
					state <= 15; // GOTO
					return;
				end
				insn <= insn_data_cached[0];
				state <= 14; // GOTO
				return;
			end
		15:
			begin
				if (((address_cached +  ((uint64_t'($bits(instruction_t)) >> 3)  ) ) == fetch_PC))
				begin
				end
				else
				begin
					state <= 17; // GOTO
					return;
				end
				insn <= insn_data_cached[1];
				state <= 16; // GOTO
				return;
			end
		17:
			begin
				$error("failed to get insn from local fetcher cache");
				assert(0);
				state <= 16; // GOTO
				return;
			end
		16:
			begin
				state <= 14; // GOTO
				return;
			end
		14:
			begin
				// CONTEXT_SWITCH();
				have_outstanding_jmp <= changesControlFlow(getOpcode(insn));
				state <= 18; // GOTO
				return;
			end
		18:
			begin
				// CONTEXT_SWITCH();
				if (decode_bus.is_busy)
				begin
					state <= 18; // GOTO
					return;
				end
				state <= 19; // GOTO
				return;
			end
		19:
			begin
				old_PC <= fetch_PC;
				fetch_PC <= fetch_PC +  ((uint64_t'($bits(instruction_t)) >> 3)  ) ;
				pkt <= create_fetch_decode_packet(exec_mask, old_PC, insn);
				decode_bus.send(pkt);
				// CONTEXT_SWITCH();
				state <= 1; // GOTO
				return;
			end
		2:
			begin
			end
	endcase
	endtask
endmodule
