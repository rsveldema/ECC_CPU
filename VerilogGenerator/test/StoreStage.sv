

function void dump_vec(input VectorValue v);
begin
begin
	for (uint32_t i = 0; (i < NUMBER_OF_VECTOR_THREADS_INT64); i=(i + 1))
		begin
			$display("VEC[", i, "] = ", v.data[i]);
		end
end
end
endfunction


module StoreStage(ExecuteToStoreBus execute_bus, VecMemoryBus memory_bus, RegisterFile regs, StoreToFetchBus fetch_bus, DivergenceQueue divergence_queue, GlobalStats stats);
	parameter CoreID core_id;
	reg[32:0] state = 0;
	ExecStagePacket pkt;
	ThreadContext new_thread_ctxt;
	VecBusPacket new_pc_pkt;



	
	
	task run();
	case (state)
		0:
			begin
				state <= 126; // GOTO
				return;
			end
		126:
			begin
				if (!(execute_bus.is_busy))
				begin
					state <= 128; // GOTO
					return;
				end
				pkt <= execute_bus.recv();
				// CONTEXT_SWITCH();
				state <= 129; // GOTO
				return;
			end
		129:
			begin
				state <= 130; // GOTO
				return;
			end
		130:
			begin
				if ((pkt.opcode == STORAGE_HALT))
				begin
				end
				else
				begin
					state <= 146; // GOTO
					return;
				end
				$display("HALT");
				if (divergence_queue.is_empty())
				begin
				end
				else
				begin
					state <= 144; // GOTO
					return;
				end
				state <= 143; // GOTO
				return;
			end
		144:
			begin
				state <= 140; // GOTO
				return;
			end
		143:
			begin
				stats.dump();
				regs.setHasHalted();
				state <= 139; // GOTO
				return;
			end
		140:
			begin
				new_thread_ctxt <= divergence_queue.get_back();
				divergence_queue.advance_read_pos();
				// CONTEXT_SWITCH();
				state <= 145; // GOTO
				return;
			end
		145:
			begin
				state <= 141; // GOTO
				return;
			end
		141:
			begin
				regs.data <= new_thread_ctxt.regs;
				fetch_bus.send(new_thread_ctxt.exec_mask, new_thread_ctxt.PC);
				state <= 139; // GOTO
				return;
			end
		139:
			begin
				state <= 131; // GOTO
				return;
				state <= 142; // GOTO
				return;
			end
		146:
			begin
				if ((pkt.opcode == STORAGE_CJMP))
				begin
				end
				else
				begin
					state <= 148; // GOTO
					return;
				end
				$display("CJMP");
				stats.incNumVectorLocalDivergences();
				$display("[STORE] splitting cond-jump: ", count_num_bits64(pkt.execution_flags_true), count_num_bits64(pkt.execution_flags_false));
				divergence_queue.push_to_front(regs.data, pkt.dest.address, pkt.execution_flags_true);
				divergence_queue.advance_write_pos();
				fetch_bus.send(pkt.execution_flags_false, pkt.src.address);
				state <= 131; // GOTO
				return;
				state <= 147; // GOTO
				return;
			end
		148:
			begin
				if ((pkt.opcode == STORAGE_JMP))
				begin
				end
				else
				begin
					state <= 150; // GOTO
					return;
				end
				fetch_bus.send(pkt.exec_mask, pkt.dest.address);
				state <= 131; // GOTO
				return;
				state <= 149; // GOTO
				return;
			end
		150:
			begin
				if ((pkt.opcode == STORAGE_LOAD_MEM_INTO_REG))
				begin
				end
				else
				begin
					state <= 157; // GOTO
					return;
				end
				assert(isValidIndex(pkt.dest.regID));
				$display("STORE-M2R ----> exec: ", pkt.dest.regID);
				dump_vec(pkt.src.value);
				memory_bus.send_read_request_vec(pkt.src.value, createBusID(core_id, COMPONENT_TYPE_STORE));
				state <= 132; // GOTO
				return;
			end
		132:
			begin
				if (memory_bus.response_busy)
				begin
				end
				else
				begin
					state <= 153; // GOTO
					return;
				end
				state <= 152; // GOTO
				return;
			end
		153:
			begin
				state <= 137; // GOTO
				return;
			end
		152:
			begin
				new_pc_pkt <= memory_bus.accept_response();
				// CONTEXT_SWITCH();
				state <= 154; // GOTO
				return;
			end
		154:
			begin
				state <= 135; // GOTO
				return;
			end
		135:
			begin
				regs.mark_valid(pkt.dest.regID);
				regs.set(pkt.dest.regID, new_pc_pkt.payload);
				if (!(!(pkt.is_store_to_pc)))
				begin
					state <= 155; // GOTO
					return;
				end
				state <= 136; // GOTO
				return;
			end
		155:
			begin
				fetch_bus.send(pkt.exec_mask, get(new_pc_pkt.payload, 0));
				state <= 136; // GOTO
				return;
			end
		136:
			begin
				state <= 133; // GOTO
				return;
				state <= 134; // GOTO
				return;
			end
		137:
			begin
				stats.incWaitForOperandFetch();
				state <= 134; // GOTO
				return;
			end
		134:
			begin
				// CONTEXT_SWITCH();
				state <= 156; // GOTO
				return;
			end
		156:
			begin
				state <= 138; // GOTO
				return;
			end
		138:
			begin
				state <= 132; // GOTO
				return;
			end
		133:
			begin
				state <= 131; // GOTO
				return;
				state <= 151; // GOTO
				return;
			end
		157:
			begin
				if ((pkt.opcode == STORAGE_STORE_REG_INTO_MEM))
				begin
				end
				else
				begin
					state <= 159; // GOTO
					return;
				end
				assert(are_all_adjacent_memory_addresses(pkt.dest.value, (int64_t'(POINTER_SIZE))));
				$display("STORE-R2M ----> exec-dest:");
				dump_vec(pkt.dest.value);
				$display("STORE-R2M ----> exec-src:");
				dump_vec(pkt.src.value);
				memory_bus.send_write_request_vec(pkt.dest.value, createBusID(core_id, COMPONENT_TYPE_STORE), pkt.src.value);
				state <= 131; // GOTO
				return;
				state <= 158; // GOTO
				return;
			end
		159:
			begin
				if ((pkt.opcode == STORAGE_STORE_VALUE_INTO_REG))
				begin
				end
				else
				begin
					state <= 161; // GOTO
					return;
				end
				assert(isValidIndex(pkt.dest.regID));
				$display("STORE-V2R ----> exec: ", pkt.dest.regID);
				dump_vec(pkt.src.value);
				regs.mark_valid(pkt.dest.regID);
				regs.set(pkt.dest.regID, pkt.src.value);
				state <= 131; // GOTO
				return;
				state <= 160; // GOTO
				return;
			end
		161:
			begin
				if ((pkt.opcode == STORAGE_NOP))
				begin
				end
				else
				begin
					state <= 163; // GOTO
					return;
				end
				state <= 131; // GOTO
				return;
				state <= 162; // GOTO
				return;
			end
		163:
			begin
				$display("unhandled store insn");
				assert(0);
				state <= 162; // GOTO
				return;
			end
		162:
			begin
				state <= 160; // GOTO
				return;
			end
		160:
			begin
				state <= 158; // GOTO
				return;
			end
		158:
			begin
				state <= 151; // GOTO
				return;
			end
		151:
			begin
				state <= 149; // GOTO
				return;
			end
		149:
			begin
				state <= 147; // GOTO
				return;
			end
		147:
			begin
				state <= 142; // GOTO
				return;
			end
		142:
			begin
				state <= 131; // GOTO
				return;
			end
		131:
			begin
				state <= 128; // GOTO
				return;
			end
		128:
			begin
				// CONTEXT_SWITCH();
				state <= 164; // GOTO
				return;
			end
		164:
			begin
				state <= 126; // GOTO
				return;
			end
		127:
			begin
			end
	endcase
	endtask
endmodule
