

function execution_mask_t get_cmp_mask(input execution_mask_t exec_mask, input VectorValue flags, input VectorValue value1);
begin
begin
	return (exec_mask & reduce_to_uint64_t(bit_and(flags, value1)));
end
end
endfunction


module ExecuteStage(DecodeToExecuteBus decode_bus, ExecuteToStoreBus store_bus, RegisterFile regs);
	parameter CoreID core_id;
	reg[32:0] state = 0;
	DecodeExecPacket pkt;



	
	
	task run();
	case (state)
		0:
			begin
				state <= 126; // GOTO
				return;
			end
		126:
			begin
				state <= 128; // GOTO
				return;
			end
		128:
			begin
				// CONTEXT_SWITCH();
				state <= 130; // GOTO
				return;
			end
		130:
			begin
				if (store_bus.is_busy)
				begin
					state <= 128; // GOTO
					return;
				end
				state <= 129; // GOTO
				return;
			end
		129:
			begin
				if (!(decode_bus.is_busy))
				begin
					state <= 131; // GOTO
					return;
				end
				pkt <= decode_bus.recv();
				// CONTEXT_SWITCH();
				state <= 132; // GOTO
				return;
			end
		132:
			begin
				$display("EXECUTE: ", pkt.PC, pkt.opcode);
				state <= 133; // GOTO
				return;
			end
		133:
			begin
				if ((pkt.opcode == EXEC_HALT))
				begin
				end
				else
				begin
					state <= 143; // GOTO
					return;
				end
				store_bus.send_none(pkt.exec_mask, pkt.PC, STORAGE_HALT);
				state <= 134; // GOTO
				return;
				state <= 142; // GOTO
				return;
			end
		143:
			begin
				if ((pkt.opcode == EXEC_JMP))
				begin
				end
				else
				begin
					state <= 145; // GOTO
					return;
				end
				store_bus.send_address(pkt.exec_mask, pkt.PC, STORAGE_JMP, (pkt.PC + get(pkt.value0.vec, 0)));
				state <= 134; // GOTO
				return;
				state <= 144; // GOTO
				return;
			end
		145:
			begin
				if ((pkt.opcode == EXEC_COND_JMP))
				begin
				end
				else
				begin
					state <= 153; // GOTO
					return;
				end
				state <= 135; // GOTO
				return;
			end
		135:
			begin
				// CONTEXT_SWITCH();
				state <= 147; // GOTO
				return;
			end
		147:
			begin
				state <= 137; // GOTO
				return;
			end
		137:
			begin
				if (!(!(regs.is_valid(REG_FLAGS))))
				begin
					state <= 148; // GOTO
					return;
				end
				state <= 135; // GOTO
				return;
			end
		148:
			begin
				state <= 136; // GOTO
				return;
			end
		136:
			begin
				if ((get_cmp_mask(pkt.exec_mask, regs.get(REG_FLAGS), pkt.value1) == 0))
				begin
				end
				else
				begin
					state <= 150; // GOTO
					return;
				end
				state <= 149; // GOTO
				return;
			end
		150:
			begin
				state <= 139; // GOTO
				return;
			end
		149:
			begin
				store_bus.send_address(pkt.exec_mask, pkt.PC, STORAGE_JMP, (pkt.PC +  ((uint64_t'($bits(instruction_t)) >> 3)  ) ));
				state <= 138; // GOTO
				return;
			end
		139:
			begin
				if ((get_cmp_mask(pkt.exec_mask, regs.get(REG_FLAGS), pkt.value1) == (pkt.exec_mask & ALL_THREADS_EXEC_MASK_INT64)))
				begin
				end
				else
				begin
					state <= 152; // GOTO
					return;
				end
				state <= 151; // GOTO
				return;
			end
		152:
			begin
				state <= 141; // GOTO
				return;
			end
		151:
			begin
				store_bus.send_address(pkt.exec_mask, pkt.PC, STORAGE_JMP, (pkt.PC + get(pkt.value0.vec, 0)));
				state <= 140; // GOTO
				return;
			end
		141:
			begin
				store_bus.send_2_address(pkt.exec_mask, pkt.PC, STORAGE_CJMP, (pkt.PC + get(pkt.value0.vec, 0)), (pkt.PC +  ((uint64_t'($bits(instruction_t)) >> 3)  ) ), get_cmp_mask(pkt.exec_mask, regs.get(REG_FLAGS), pkt.value1), (pkt.exec_mask & ~(get_cmp_mask(pkt.exec_mask, regs.get(REG_FLAGS), pkt.value1))));
				state <= 140; // GOTO
				return;
			end
		140:
			begin
				state <= 138; // GOTO
				return;
			end
		138:
			begin
				state <= 134; // GOTO
				return;
				state <= 146; // GOTO
				return;
			end
		153:
			begin
				if ((pkt.opcode == EXEC_LOAD_RESTORE_PC))
				begin
				end
				else
				begin
					state <= 155; // GOTO
					return;
				end
				store_bus.send_reg_vec(pkt.exec_mask, pkt.PC, STORAGE_LOAD_MEM_INTO_REG, REG_PC, add(pkt.value0.vec, pkt.value1), 1);
				state <= 134; // GOTO
				return;
				state <= 154; // GOTO
				return;
			end
		155:
			begin
				if ((pkt.opcode == EXEC_STORE_ADDR_VALUE))
				begin
				end
				else
				begin
					state <= 157; // GOTO
					return;
				end
				store_bus.send_vec_vec(pkt.exec_mask, pkt.PC, STORAGE_STORE_REG_INTO_MEM, add(pkt.value1, pkt.value2), pkt.value0.vec);
				state <= 134; // GOTO
				return;
				state <= 156; // GOTO
				return;
			end
		157:
			begin
				if ((pkt.opcode == EXEC_LOAD_REG))
				begin
				end
				else
				begin
					state <= 159; // GOTO
					return;
				end
				regs.mark_invalid(pkt.value0.regID);
				store_bus.send_reg_vec(pkt.exec_mask, pkt.PC, STORAGE_LOAD_MEM_INTO_REG, pkt.value0.regID, add(pkt.value1, pkt.value2), 0);
				state <= 134; // GOTO
				return;
				state <= 158; // GOTO
				return;
			end
		159:
			begin
				if ((pkt.opcode == EXEC_ADD_REG_VALUE_VALUE))
				begin
				end
				else
				begin
					state <= 161; // GOTO
					return;
				end
				regs.mark_invalid(pkt.value0.regID);
				store_bus.send_reg_vec(pkt.exec_mask, pkt.PC, STORAGE_STORE_VALUE_INTO_REG, pkt.value0.regID, add(pkt.value1, pkt.value2), 0);
				state <= 134; // GOTO
				return;
				state <= 160; // GOTO
				return;
			end
		161:
			begin
				if ((pkt.opcode == EXEC_SHL_REG_VALUE_VALUE))
				begin
				end
				else
				begin
					state <= 163; // GOTO
					return;
				end
				regs.mark_invalid(pkt.value0.regID);
				store_bus.send_reg_vec(pkt.exec_mask, pkt.PC, STORAGE_STORE_VALUE_INTO_REG, pkt.value0.regID, shift_left(pkt.value1, pkt.value2), 0);
				state <= 134; // GOTO
				return;
				state <= 162; // GOTO
				return;
			end
		163:
			begin
				if ((pkt.opcode == EXEC_ORC_REG_VALUE))
				begin
				end
				else
				begin
					state <= 165; // GOTO
					return;
				end
				regs.mark_invalid(pkt.value0.regID);
				store_bus.send_reg_vec(pkt.exec_mask, pkt.PC, STORAGE_STORE_VALUE_INTO_REG, pkt.value0.regID, or_shift_left(pkt.value2, pkt.value1, 48), 0);
				state <= 134; // GOTO
				return;
				state <= 164; // GOTO
				return;
			end
		165:
			begin
				if ((pkt.opcode == EXEC_ORB_REG_VALUE))
				begin
				end
				else
				begin
					state <= 167; // GOTO
					return;
				end
				regs.mark_invalid(pkt.value0.regID);
				store_bus.send_reg_vec(pkt.exec_mask, pkt.PC, STORAGE_STORE_VALUE_INTO_REG, pkt.value0.regID, or_shift_left(pkt.value2, pkt.value1, 24), 0);
				state <= 134; // GOTO
				return;
				state <= 166; // GOTO
				return;
			end
		167:
			begin
				if ((pkt.opcode == EXEC_MOVE_REG_VALUE))
				begin
				end
				else
				begin
					state <= 169; // GOTO
					return;
				end
				regs.mark_invalid(pkt.value0.regID);
				store_bus.send_reg_vec(pkt.exec_mask, pkt.PC, STORAGE_STORE_VALUE_INTO_REG, pkt.value0.regID, pkt.value1, 0);
				state <= 134; // GOTO
				return;
				state <= 168; // GOTO
				return;
			end
		169:
			begin
				if ((pkt.opcode == EXEC_CMP))
				begin
				end
				else
				begin
					state <= 171; // GOTO
					return;
				end
				regs.mark_invalid(REG_FLAGS);
				store_bus.send_reg_vec(pkt.exec_mask, pkt.PC, STORAGE_STORE_VALUE_INTO_REG, REG_FLAGS, compare_vecs(pkt.value0.vec, pkt.value1), 0);
				state <= 134; // GOTO
				return;
				state <= 170; // GOTO
				return;
			end
		171:
			begin
				if ((pkt.opcode == EXEC_NOP))
				begin
				end
				else
				begin
					state <= 173; // GOTO
					return;
				end
				state <= 134; // GOTO
				return;
				state <= 172; // GOTO
				return;
			end
		173:
			begin
				$display("[EXECUTE] unhandled opcode in execute stage: ", pkt.opcode);
				assert(0);
				state <= 172; // GOTO
				return;
			end
		172:
			begin
				state <= 170; // GOTO
				return;
			end
		170:
			begin
				state <= 168; // GOTO
				return;
			end
		168:
			begin
				state <= 166; // GOTO
				return;
			end
		166:
			begin
				state <= 164; // GOTO
				return;
			end
		164:
			begin
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
				state <= 156; // GOTO
				return;
			end
		156:
			begin
				state <= 154; // GOTO
				return;
			end
		154:
			begin
				state <= 146; // GOTO
				return;
			end
		146:
			begin
				state <= 144; // GOTO
				return;
			end
		144:
			begin
				state <= 142; // GOTO
				return;
			end
		142:
			begin
				state <= 134; // GOTO
				return;
			end
		134:
			begin
				state <= 131; // GOTO
				return;
			end
		131:
			begin
				// CONTEXT_SWITCH();
				state <= 174; // GOTO
				return;
			end
		174:
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
