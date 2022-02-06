

typedef union packed {
	uint64_t value;
	fetched_instruction_data_t data;
} int64_to_insn_data;


function getInsnData(BusPacket pkt);
	reg[32:0] state = 0;
	int64_to_insn_data tmp = 0;
begin
	tmp=pkt;
	return tmp;
end
endfunction


function changesControlFlow(Opcode op);
	reg[32:0] state = 0;
begin
	case (op)
		HALT,
		JMP_ALWAYS,
		JMP_EQUAL,
		JMP_NOT_EQUAL,
		JMP_GREATER,
		JMP_GREATER_EQUAL,
		JMP_LOWER,
		JMP_LOWER_EQUAL,
		LOAD_RESTORE_PC:
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
endfunction


module FetchStage(FetchToDecodeBus decode_bus, StoreToFetchBus store_bus, MemoryBus memory_bus);
	reg[32:0] state = 0;
	bool have_outstanding_jmp = 0;
	memory_address_t fetch_PC = 0;
	execution_mask_t exec_mask = 0;
	memory_address_t address_cached = 0;
	fetched_instruction_data_t fetched_cached = 0;
	StoreToFetchPacket jmp_retarget = 0;
	memory_address_t address_fetched = 0;
	BusPacket response = 0;
	instruction_t insn = 0;
	Opcode opcode = 0;
	memory_address_t PC = 0;
	FetchToDecodeBusPacket pkt = 0;



	
	
	task run();
	case (state)
		0:
			begin
				have_outstanding_jmp = 0;
				fetch_PC = 0;
				// local_obj execution_mask_t exec_mask(ALL_THREADS_EXEC_MASK_INT64)
				address_cached = 'hffffffff;
				// local_obj fetched_instruction_data_t fetched_cached()
				state = 21; // GOTO
				return;
			end
		21:
			begin
				if (!(have_outstanding_jmp))
				begin
					state = 23; // GOTO
					return;
				end
				have_outstanding_jmp=0;
				state = 24; // GOTO
				return;
			end
		24:
			begin
				if (!(store_bus))
				begin
					state = 26; // GOTO
					return;
				end
				jmp_retarget = store_bus();
				fetch_PC=jmp_retarget;
				exec_mask=jmp_retarget;
				state = 25; // GOTO
				return;
			end
		26:
			begin
				// CONTEXT_SWITCH()
				state = 24; // GOTO
				return;
			end
		25:
			begin
				state = 23; // GOTO
				return;
			end
		23:
			begin
				if ((address_cached == fetch_PC))
				begin
				end
				else
				begin
					state = 27; // GOTO
					return;
				end
				state = 27; // GOTO
				return;
			end
		28:
			begin
				if (((address_cached + ($bits(instruction_t) / 8)) == fetch_PC))
				begin
				end
				else
				begin
					state = 29; // GOTO
					return;
				end
				state = 29; // GOTO
				return;
			end
		30:
			begin
				address_fetched = (fetch_PC & ~(7));
				// memory_bus(address_fetched, memory_bus_id)
				state = 31; // GOTO
				return;
			end
		31:
			begin
				if (!(memory_bus))
				begin
					state = 33; // GOTO
					return;
				end
				response = memory_bus();
				// assert((response == BusPacketType::read_response))
				address_cached=address_fetched;
				fetched_cached=getInsnData(response);
				state = 32; // GOTO
				return;
			end
		33:
			begin
				// stats()
				// CONTEXT_SWITCH()
				state = 31; // GOTO
				return;
			end
		32:
			begin
				state = 29; // GOTO
				return;
			end
		29:
			begin
				state = 27; // GOTO
				return;
			end
		27:
			begin
				insn = 0;
				if ((address_cached == fetch_PC))
				begin
				end
				else
				begin
					state = 34; // GOTO
					return;
				end
				insn=fetched_cached[0];
				state = 34; // GOTO
				return;
			end
		35:
			begin
				if (((address_cached + ($bits(instruction_t) / 8)) == fetch_PC))
				begin
				end
				else
				begin
					state = 36; // GOTO
					return;
				end
				insn=fetched_cached[1];
				state = 36; // GOTO
				return;
			end
		37:
			begin
				// logger("failed to get insn from local fetcher cache")
				// abort()
				state = 36; // GOTO
				return;
			end
		36:
			begin
				state = 34; // GOTO
				return;
			end
		34:
			begin
				opcode = (Opcode'((insn & 'hff)));
				if (!(changesControlFlow(opcode)))
				begin
					state = 38; // GOTO
					return;
				end
				have_outstanding_jmp=1;
				state = 38; // GOTO
				return;
			end
		38:
			begin
				state = 39; // GOTO
				return;
			end
		39:
			begin
				// CONTEXT_SWITCH()
				if (decode_bus)
				begin
					state = 39; // GOTO
					return;
				end
				state = 40; // GOTO
				return;
			end
		40:
			begin
				PC = fetch_PC;
				fetch_PC+=($bits(instruction_t) / 8);
				// local_obj FetchToDecodeBusPacket pkt(exec_mask, PC, insn)
				// decode_bus(pkt)
				// CONTEXT_SWITCH()
				state = 21; // GOTO
				return;
			end
		22:
			begin
			end
	endcase
	endtask
endmodule
