

typedef union packed {
	uint64_t value;
	fetched_instruction_data_t data;
} int64_to_insn_data;


function fetched_instruction_data_t getInsnData(input uint64_t value);
begin
	int64_to_insn_data tmp;
begin
	tmp.value=value;
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
	BusID memory_bus_id;
	BusPacket response;
	instruction_t insn;
	Opcode opcode;
	memory_address_t PC;
	FetchToDecodeBusPacket pkt;



	
	
	task run();
	case (state)
		0:
			begin
				have_outstanding_jmp = 0;
				fetch_PC = 0;
				exec_mask = ALL_THREADS_EXEC_MASK_INT64;
				address_cached = 64'hffffffffffffffff;
				state = 1; // GOTO
				return;
			end
		1:
			begin
				if (!(have_outstanding_jmp))
				begin
					state = 3; // GOTO
					return;
				end
				have_outstanding_jmp=0;
				state = 4; // GOTO
				return;
			end
		4:
			begin
				if (!(store_bus.can_receive))
				begin
					state = 6; // GOTO
					return;
				end
				jmp_retarget = store_bus.recv();
				fetch_PC=jmp_retarget.newpc;
				exec_mask=jmp_retarget.exec_mask;
				state = 5; // GOTO
				return;
			end
		6:
			begin
				CONTEXT_SWITCH();
				state = 4; // GOTO
				return;
			end
		5:
			begin
				state = 3; // GOTO
				return;
			end
		3:
			begin
				if ((address_cached == fetch_PC))
				begin
				end
				else
				begin
					state = 7; // GOTO
					return;
				end
				state = 7; // GOTO
				return;
			end
		8:
			begin
				if (((address_cached +  ((uint64_t'($bits(instruction_t)) >> 3)  ) ) == fetch_PC))
				begin
				end
				else
				begin
					state = 9; // GOTO
					return;
				end
				state = 9; // GOTO
				return;
			end
		10:
			begin
				address_fetched = (fetch_PC & ~(7));
				memory_bus_id = createBusID(core_id, COMPONENT_TYPE_FETCH);
				memory_bus.send_read_request_data(address_fetched, memory_bus_id);
				state = 11; // GOTO
				return;
			end
		11:
			begin
				if (!(memory_bus.response_busy))
				begin
					state = 13; // GOTO
					return;
				end
				response = memory_bus.get_response();
				assert((response.packet_type == bus_read_response));
				address_cached=address_fetched;
				insn_data_cached=getInsnData(response.payload);
				state = 12; // GOTO
				return;
			end
		13:
			begin
				incFetchedInsnWait();
				CONTEXT_SWITCH();
				state = 11; // GOTO
				return;
			end
		12:
			begin
				state = 9; // GOTO
				return;
			end
		9:
			begin
				state = 7; // GOTO
				return;
			end
		7:
			begin
				insn = 0;
				if ((address_cached == fetch_PC))
				begin
				end
				else
				begin
					state = 14; // GOTO
					return;
				end
				insn=insn_data_cached[0];
				state = 14; // GOTO
				return;
			end
		15:
			begin
				if (((address_cached +  ((uint64_t'($bits(instruction_t)) >> 3)  ) ) == fetch_PC))
				begin
				end
				else
				begin
					state = 16; // GOTO
					return;
				end
				insn=insn_data_cached[1];
				state = 16; // GOTO
				return;
			end
		17:
			begin
				error("failed to get insn from local fetcher cache");
				assert(0);
				state = 16; // GOTO
				return;
			end
		16:
			begin
				state = 14; // GOTO
				return;
			end
		14:
			begin
				opcode = (Opcode'((insn & 32'hff)));
				if (!(changesControlFlow(opcode)))
				begin
					state = 18; // GOTO
					return;
				end
				have_outstanding_jmp=1;
				state = 18; // GOTO
				return;
			end
		18:
			begin
				state = 19; // GOTO
				return;
			end
		19:
			begin
				CONTEXT_SWITCH();
				if (decode_bus.is_busy)
				begin
					state = 19; // GOTO
					return;
				end
				state = 20; // GOTO
				return;
			end
		20:
			begin
				PC = fetch_PC;
				fetch_PC+= ((uint64_t'($bits(instruction_t)) >> 3)  ) ;
				pkt = create_fetch_decode_packet(exec_mask, PC, insn);
				decode_bus.send(pkt);
				CONTEXT_SWITCH();
				state = 1; // GOTO
				return;
			end
		2:
			begin
			end
	endcase
	endtask
endmodule
