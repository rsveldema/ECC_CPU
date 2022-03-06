

function execution_mask_t get_cmp_mask(input execution_mask_t exec_mask, input VectorValue flags, input VectorValue value1);
begin
begin
	return (exec_mask & reduce_to_uint64_t(bit_and(flags, value1)));
end
end
endfunction


module ExecuteStage(DecodeToExecuteBus decode_bus, ExecuteToStoreBus store_bus, RegisterFile regs, GlobalStats stats);
	parameter CoreID core_id;
	reg[32:0] state = 0;
	DecodeExecPacket pkt;



	
	
	task run();
	case (state)
		0:
			begin
				state <= 165; // GOTO
				return;
			end
		165:
			begin
				stats.incNumCyclesUsed();
				state <= 167; // GOTO
				return;
			end
		167:
			begin
				// CONTEXT_SWITCH();
				state <= 169; // GOTO
				return;
			end
		169:
			begin
				if (store_bus.is_busy)
				begin
					state <= 167; // GOTO
					return;
				end
				state <= 168; // GOTO
				return;
			end
		168:
			begin
				if (!(decode_bus.is_busy))
				begin
					state <= 170; // GOTO
					return;
				end
				pkt <= decode_bus.recv();
				// CONTEXT_SWITCH();
				state <= 171; // GOTO
				return;
			end
		171:
			begin
				$display("[EXECUTE] ", pkt.PC, pkt.opcode);
				state <= 172; // GOTO
				return;
			end
		172:
			begin
				if ((pkt.opcode == EXEC_HALT))
				begin
				end
				else
				begin
					state <= 182; // GOTO
					return;
				end
				store_bus.send_none(pkt.exec_mask, pkt.PC, STORAGE_HALT);
				state <= 173; // GOTO
				return;
				state <= 181; // GOTO
				return;
			end
		182:
			begin
				if ((pkt.opcode == EXEC_JMP))
				begin
				end
				else
				begin
					state <= 184; // GOTO
					return;
				end
				store_bus.send_address(pkt.exec_mask, pkt.PC, STORAGE_JMP, (pkt.PC + get(pkt.value0.vec, 0)));
				state <= 173; // GOTO
				return;
				state <= 183; // GOTO
				return;
			end
		184:
			begin
				if ((pkt.opcode == EXEC_COND_JMP))
				begin
				end
				else
				begin
					state <= 192; // GOTO
					return;
				end
				state <= 174; // GOTO
				return;
			end
		174:
			begin
				// CONTEXT_SWITCH();
				state <= 186; // GOTO
				return;
			end
		186:
			begin
				state <= 176; // GOTO
				return;
			end
		176:
			begin
				if (!(!(regs.is_valid(REG_FLAGS))))
				begin
					state <= 187; // GOTO
					return;
				end
				state <= 174; // GOTO
				return;
			end
		187:
			begin
				state <= 175; // GOTO
				return;
			end
		175:
			begin
				if ((get_cmp_mask(pkt.exec_mask, regs.get(REG_FLAGS), pkt.value1) == 0))
				begin
				end
				else
				begin
					state <= 189; // GOTO
					return;
				end
				state <= 188; // GOTO
				return;
			end
		189:
			begin
				state <= 178; // GOTO
				return;
			end
		188:
			begin
				store_bus.send_address(pkt.exec_mask, pkt.PC, STORAGE_JMP, (pkt.PC +  ((uint64_t'($bits(instruction_t)) >> 3)  ) ));
				state <= 177; // GOTO
				return;
			end
		178:
			begin
				if ((get_cmp_mask(pkt.exec_mask, regs.get(REG_FLAGS), pkt.value1) == (pkt.exec_mask & ALL_THREADS_EXEC_MASK_INT64)))
				begin
				end
				else
				begin
					state <= 191; // GOTO
					return;
				end
				state <= 190; // GOTO
				return;
			end
		191:
			begin
				state <= 180; // GOTO
				return;
			end
		190:
			begin
				store_bus.send_address(pkt.exec_mask, pkt.PC, STORAGE_JMP, (pkt.PC + get(pkt.value0.vec, 0)));
				state <= 179; // GOTO
				return;
			end
		180:
			begin
				store_bus.send_2_address(pkt.exec_mask, pkt.PC, STORAGE_CJMP, (pkt.PC + get(pkt.value0.vec, 0)), (pkt.PC +  ((uint64_t'($bits(instruction_t)) >> 3)  ) ), get_cmp_mask(pkt.exec_mask, regs.get(REG_FLAGS), pkt.value1), (pkt.exec_mask & ~(get_cmp_mask(pkt.exec_mask, regs.get(REG_FLAGS), pkt.value1))));
				state <= 179; // GOTO
				return;
			end
		179:
			begin
				state <= 177; // GOTO
				return;
			end
		177:
			begin
				state <= 173; // GOTO
				return;
				state <= 185; // GOTO
				return;
			end
		192:
			begin
				if ((pkt.opcode == EXEC_LOAD_RESTORE_PC))
				begin
				end
				else
				begin
					state <= 194; // GOTO
					return;
				end
				store_bus.send_reg_vec(pkt.exec_mask, pkt.PC, STORAGE_LOAD_MEM_INTO_REG, REG_PC, add(pkt.value0.vec, pkt.value1), 1);
				state <= 173; // GOTO
				return;
				state <= 193; // GOTO
				return;
			end
		194:
			begin
				if ((pkt.opcode == EXEC_STORE_ADDR_VALUE))
				begin
				end
				else
				begin
					state <= 196; // GOTO
					return;
				end
				store_bus.send_vec_vec(pkt.exec_mask, pkt.PC, STORAGE_STORE_REG_INTO_MEM, add(pkt.value1, pkt.value2), pkt.value0.vec);
				state <= 173; // GOTO
				return;
				state <= 195; // GOTO
				return;
			end
		196:
			begin
				if ((pkt.opcode == EXEC_LOAD_REG))
				begin
				end
				else
				begin
					state <= 198; // GOTO
					return;
				end
				regs.mark_invalid(pkt.value0.regID);
				store_bus.send_reg_vec(pkt.exec_mask, pkt.PC, STORAGE_LOAD_MEM_INTO_REG, pkt.value0.regID, add(pkt.value1, pkt.value2), 0);
				state <= 173; // GOTO
				return;
				state <= 197; // GOTO
				return;
			end
		198:
			begin
				if ((pkt.opcode == EXEC_ADD_REG_VALUE_VALUE))
				begin
				end
				else
				begin
					state <= 200; // GOTO
					return;
				end
				regs.mark_invalid(pkt.value0.regID);
				store_bus.send_reg_vec(pkt.exec_mask, pkt.PC, STORAGE_STORE_VALUE_INTO_REG, pkt.value0.regID, add(pkt.value1, pkt.value2), 0);
				state <= 173; // GOTO
				return;
				state <= 199; // GOTO
				return;
			end
		200:
			begin
				if ((pkt.opcode == EXEC_SHL_REG_VALUE_VALUE))
				begin
				end
				else
				begin
					state <= 202; // GOTO
					return;
				end
				regs.mark_invalid(pkt.value0.regID);
				store_bus.send_reg_vec(pkt.exec_mask, pkt.PC, STORAGE_STORE_VALUE_INTO_REG, pkt.value0.regID, shift_left(pkt.value1, pkt.value2), 0);
				state <= 173; // GOTO
				return;
				state <= 201; // GOTO
				return;
			end
		202:
			begin
				if ((pkt.opcode == EXEC_ORC_REG_VALUE))
				begin
				end
				else
				begin
					state <= 204; // GOTO
					return;
				end
				regs.mark_invalid(pkt.value0.regID);
				store_bus.send_reg_vec(pkt.exec_mask, pkt.PC, STORAGE_STORE_VALUE_INTO_REG, pkt.value0.regID, or_shift_left(pkt.value2, pkt.value1, 48), 0);
				state <= 173; // GOTO
				return;
				state <= 203; // GOTO
				return;
			end
		204:
			begin
				if ((pkt.opcode == EXEC_ORB_REG_VALUE))
				begin
				end
				else
				begin
					state <= 206; // GOTO
					return;
				end
				regs.mark_invalid(pkt.value0.regID);
				store_bus.send_reg_vec(pkt.exec_mask, pkt.PC, STORAGE_STORE_VALUE_INTO_REG, pkt.value0.regID, or_shift_left(pkt.value2, pkt.value1, 24), 0);
				state <= 173; // GOTO
				return;
				state <= 205; // GOTO
				return;
			end
		206:
			begin
				if ((pkt.opcode == EXEC_MOVE_REG_VALUE))
				begin
				end
				else
				begin
					state <= 208; // GOTO
					return;
				end
				regs.mark_invalid(pkt.value0.regID);
				store_bus.send_reg_vec(pkt.exec_mask, pkt.PC, STORAGE_STORE_VALUE_INTO_REG, pkt.value0.regID, pkt.value1, 0);
				state <= 173; // GOTO
				return;
				state <= 207; // GOTO
				return;
			end
		208:
			begin
				if ((pkt.opcode == EXEC_CMP))
				begin
				end
				else
				begin
					state <= 210; // GOTO
					return;
				end
				regs.mark_invalid(REG_FLAGS);
				store_bus.send_reg_vec(pkt.exec_mask, pkt.PC, STORAGE_STORE_VALUE_INTO_REG, REG_FLAGS, compare_vecs(pkt.value0.vec, pkt.value1), 0);
				state <= 173; // GOTO
				return;
				state <= 209; // GOTO
				return;
			end
		210:
			begin
				if ((pkt.opcode == EXEC_NOP))
				begin
				end
				else
				begin
					state <= 212; // GOTO
					return;
				end
				state <= 173; // GOTO
				return;
				state <= 211; // GOTO
				return;
			end
		212:
			begin
				$display("[EXECUTE] unhandled opcode in execute stage: ", pkt.opcode);
				assert(0);
				state <= 211; // GOTO
				return;
			end
		211:
			begin
				state <= 209; // GOTO
				return;
			end
		209:
			begin
				state <= 207; // GOTO
				return;
			end
		207:
			begin
				state <= 205; // GOTO
				return;
			end
		205:
			begin
				state <= 203; // GOTO
				return;
			end
		203:
			begin
				state <= 201; // GOTO
				return;
			end
		201:
			begin
				state <= 199; // GOTO
				return;
			end
		199:
			begin
				state <= 197; // GOTO
				return;
			end
		197:
			begin
				state <= 195; // GOTO
				return;
			end
		195:
			begin
				state <= 193; // GOTO
				return;
			end
		193:
			begin
				state <= 185; // GOTO
				return;
			end
		185:
			begin
				state <= 183; // GOTO
				return;
			end
		183:
			begin
				state <= 181; // GOTO
				return;
			end
		181:
			begin
				state <= 173; // GOTO
				return;
			end
		173:
			begin
				state <= 170; // GOTO
				return;
			end
		170:
			begin
				// CONTEXT_SWITCH();
				state <= 213; // GOTO
				return;
			end
		213:
			begin
				state <= 165; // GOTO
				return;
			end
		166:
			begin
			end
	endcase
	endtask
endmodule
