function changesControlFlow;
	reg[32:0] state = 0;
begin
	case (op)
		HALT:
		JMP_ALWAYS:
		JMP_EQUAL:
		JMP_NOT_EQUAL:
		JMP_GREATER:
		JMP_GREATER_EQUAL:
		JMP_LOWER:
		JMP_LOWER_EQUAL:
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
module FetchStage;
	reg[32:0] state = 0;
	bool have_outstanding_jmp = 0;
	memory_address_t fetch_PC = 0;
	ExecutionMask exec_mask = 0;
	memory_address_t address_cached = 0;
	fetched_instruction_data_t fetched_cached = 0;
	StoreToFetchBus__Packet jmp_retarget = 0;
	memory_address_t address_fetched = 0;
	InsnCacheMemoryBus__Packet response = 0;
	instruction_t insn = 0;
	Opcode opcode = 0;
	memory_address_t PC = 0;
	FetchToDecodeBus__Packet pkt = 0;



	task run;
	case (state)
		0:
			begin
				have_outstanding_jmp = 0;
				fetch_PC = 0;
				// local_obj ExecutionMask exec_mask(ALL_THREADS_EXEC_MASK_INT64)
				address_cached = 'hffffffff;
				// local_obj fetched_instruction_data_t fetched_cached()
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
				if (!(store_bus()))
				begin
					state = 6; // GOTO
					return;
				end
				jmp_retarget = store_bus();
				fetch_PC=jmp_retarget;
				exec_mask=jmp_retarget;
				state = 5; // GOTO
				return;
			end
		6:
			begin
				// CONTEXT_SWITCH()
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
				if (((address_cached + ($bits(instruction_t) / 8)) == fetch_PC))
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
				// memory_bus(address_fetched, memory_bus_id)
				state = 11; // GOTO
				return;
			end
		11:
			begin
				if (!(memory_bus()))
				begin
					state = 13; // GOTO
					return;
				end
				response = memory_bus();
				// assert((response == InsnCacheMemoryBus::Type::read_response))
				address_cached=address_fetched;
				fetched_cached=response();
				state = 12; // GOTO
				return;
			end
		13:
			begin
				// stats()
				// CONTEXT_SWITCH()
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
				insn=fetched_cached[0];
				state = 14; // GOTO
				return;
			end
		15:
			begin
				if (((address_cached + ($bits(instruction_t) / 8)) == fetch_PC))
				begin
				end
				else
				begin
					state = 16; // GOTO
					return;
				end
				insn=fetched_cached[1];
				state = 16; // GOTO
				return;
			end
		17:
			begin
				// logger("failed to get insn from local fetcher cache")
				// abort()
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
				opcode = (Opcode'((insn & 'hff)));
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
				// CONTEXT_SWITCH()
				if (decode_bus())
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
				fetch_PC+=($bits(instruction_t) / 8);
				// local_obj FetchToDecodeBus__Packet pkt(exec_mask, PC, insn)
				// decode_bus(pkt)
				// CONTEXT_SWITCH()
				state = 1; // GOTO
				return;
			end
		2:
			begin
			end
	endcase
	endtask
endmodule
