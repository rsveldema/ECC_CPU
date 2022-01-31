function changesControlFlow;
endfunction
module FetchStage;
	reg[32:0] state = 0;
	bool have_outstanding_jmp = 0;
	memory_address_t fetch_pc = 0;
	ExecutionMask exec_mask = 0;
	memory_address_t address_cached = 0;
	fetched_instruction_data_t fetched_cached = 0;
	StoreToFetchBus__Packet jmp_retarget = 0;
	memory_address_t address_fetched = 0;
	InsnCacheMemoryBus__Packet response = 0;
	instruction_t insn = 0;
	Opcode opcode = 0;
	memory_address_t pc = 0;
	FetchToDecodeBus__Packet pkt = 0;



	task run;
	case (state)
		0:
			begin
				have_outstanding_jmp = 0;
				fetch_pc = 0;
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
				if (!(call store_bus()))
				begin
					state = 6; // GOTO
					return;
				end
				jmp_retarget = call store_bus();
				fetch_PC=jmp_retarget;
				exec_mask=jmp_retarget;
				state = 5; // GOTO
				return;
			end
		6:
			begin
				// call CONTEXT_SWITCH()
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
				if (((address_cached + call sizeof(instruction_t)) == fetch_PC))
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
				// call memory_bus(address_fetched, memory_bus_id)
				state = 11; // GOTO
				return;
			end
		11:
			begin
				if (!(call memory_bus()))
				begin
					state = 13; // GOTO
					return;
				end
				response = call memory_bus();
				// call assert((response == InsnCacheMemoryBus::Type::read_response))
				address_cached=address_fetched;
				fetched_cached=call response();
				state = 12; // GOTO
				return;
			end
		13:
			begin
				// call stats()
				// call CONTEXT_SWITCH()
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
				if (((address_cached + INSTRUCTION_SIZE) == fetch_PC))
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
				// call logger("failed to get insn from local fetcher cache")
				// call abort()
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
				if (!(call changesControlFlow(opcode)))
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
				// call CONTEXT_SWITCH()
				if (call decode_bus())
				begin
					state = 19; // GOTO
					return;
				end
				state = 20; // GOTO
				return;
			end
		20:
			begin
				pc = fetch_PC;
				fetch_PC+=INSTRUCTION_SIZE;
				// local_obj FetchToDecodeBus__Packet pkt(exec_mask, PC, insn)
				// call decode_bus(pkt)
				// call CONTEXT_SWITCH()
				state = 1; // GOTO
				return;
			end
		2:
			begin
			end
	endcase
	endtask
endmodule
