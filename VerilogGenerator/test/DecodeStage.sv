

function uint64_t get_expected_cond_jump_flags_mask(input Opcode opcode);
begin
begin
	case (opcode)
		INSN_OPCODE_JMP_EQUAL:
			begin
				return FLAGS_MASK_EQ;
			end
		INSN_OPCODE_JMP_NOT_EQUAL:
			begin
				return (FLAGS_MASK_LT | FLAGS_MASK_GT);
			end
		INSN_OPCODE_JMP_GREATER:
			begin
				return FLAGS_MASK_GT;
			end
		INSN_OPCODE_JMP_LOWER:
			begin
				return FLAGS_MASK_LT;
			end
		INSN_OPCODE_JMP_GREATER_EQUAL:
			begin
				return (FLAGS_MASK_GT | FLAGS_MASK_EQ);
			end
		INSN_OPCODE_JMP_LOWER_EQUAL:
			begin
				return (FLAGS_MASK_LT | FLAGS_MASK_EQ);
			end
		default:
			begin
				assert(0);
				return FLAGS_MASK_GT;
			end
	endcase
	assert(0);
	return FLAGS_MASK_GT;
end
end
endfunction


module DecodeStage(FetchToDecodeBus fetch_bus, DecodeToExecuteBus execute_bus, RegisterFile regs);
	parameter CoreID core_id;
	reg[32:0] state = 0;
	DecodeStageValue value0;
	VectorValue value1;
	VectorValue value2;
	FetchToDecodeBusPacket pkt;
	execution_mask_t expected_mask;



	
	
	task run();
	case (state)
		0:
			begin
				state <= 30; // GOTO
				return;
			end
		30:
			begin
				if (!(fetch_bus.is_busy))
				begin
					state <= 32; // GOTO
					return;
				end
				pkt <= fetch_bus.recv();
				// CONTEXT_SWITCH();
				state <= 33; // GOTO
				return;
			end
		33:
			begin
				$display("DECODE[%d] exec: %x", pkt.PC, (Opcode'((pkt.insn & 32'hff))));
				state <= 34; // GOTO
				return;
			end
		34:
			begin
				if (((Opcode'((pkt.insn & 32'hff))) == INSN_OPCODE_MOVE_R0_CONST24C))
				begin
				end
				else
				begin
					state <= 40; // GOTO
					return;
				end
				value0.regID <= REG_R0;
				value1 <= create_vec_int64((int32_t'((pkt.insn >> 8))));
				value2 <= regs.get(REG_R0);
				execute_bus.send_req3(pkt.exec_mask, pkt.PC, EXEC_ORC_REG_VALUE, value0, value1, value2);
				state <= 35; // GOTO
				return;
				state <= 39; // GOTO
				return;
			end
		40:
			begin
				if (((Opcode'((pkt.insn & 32'hff))) == INSN_OPCODE_MOVE_R0_CONST24B))
				begin
				end
				else
				begin
					state <= 42; // GOTO
					return;
				end
				value0.regID <= REG_R0;
				value1 <= create_vec_int64((int32_t'((pkt.insn >> 8))));
				value2 <= regs.get(REG_R0);
				execute_bus.send_req3(pkt.exec_mask, pkt.PC, EXEC_ORB_REG_VALUE, value0, value1, value2);
				state <= 35; // GOTO
				return;
				state <= 41; // GOTO
				return;
			end
		42:
			begin
				if (((Opcode'((pkt.insn & 32'hff))) == INSN_OPCODE_MOVE_R0_CONST24A))
				begin
				end
				else
				begin
					state <= 44; // GOTO
					return;
				end
				value0.regID <= REG_R0;
				value1 <= create_vec_int64((int32_t'((pkt.insn >> 8))));
				execute_bus.send_req2(pkt.exec_mask, pkt.PC, EXEC_MOVE_REG_VALUE, value0, value1);
				state <= 35; // GOTO
				return;
				state <= 43; // GOTO
				return;
			end
		44:
			begin
				if (((Opcode'((pkt.insn & 32'hff))) == INSN_OPCODE_LOAD_REG_CONST_REG))
				begin
				end
				else
				begin
					state <= 46; // GOTO
					return;
				end
				value0.regID <= (RegisterID'(((pkt.insn >> 8) & 32'hff)));
				value1 <= create_vec_int64((int16_t'((pkt.insn >> 24))));
				value2 <= regs.get((RegisterID'(((pkt.insn >> 16) & 32'hff))));
				execute_bus.send_req3(pkt.exec_mask, pkt.PC, EXEC_LOAD_REG, value0, value1, value2);
				state <= 35; // GOTO
				return;
				state <= 45; // GOTO
				return;
			end
		46:
			begin
				if (((Opcode'((pkt.insn & 32'hff))) == INSN_OPCODE_LOAD_RESTORE_PC))
				begin
				end
				else
				begin
					state <= 48; // GOTO
					return;
				end
				value0.vec <= create_vec_int64((int16_t'((pkt.insn >> 16))));
				value1 <= regs.get((RegisterID'(((pkt.insn >> 8) & 32'hff))));
				execute_bus.send_req2(pkt.exec_mask, pkt.PC, EXEC_LOAD_RESTORE_PC, value0, value1);
				state <= 35; // GOTO
				return;
				state <= 47; // GOTO
				return;
			end
		48:
			begin
				if ((((((((Opcode'((pkt.insn & 32'hff))) == INSN_OPCODE_JMP_LOWER) || ((Opcode'((pkt.insn & 32'hff))) == INSN_OPCODE_JMP_LOWER_EQUAL)) || ((Opcode'((pkt.insn & 32'hff))) == INSN_OPCODE_JMP_EQUAL)) || ((Opcode'((pkt.insn & 32'hff))) == INSN_OPCODE_JMP_NOT_EQUAL)) || ((Opcode'((pkt.insn & 32'hff))) == INSN_OPCODE_JMP_GREATER)) || ((Opcode'((pkt.insn & 32'hff))) == INSN_OPCODE_JMP_GREATER_EQUAL)))
				begin
				end
				else
				begin
					state <= 52; // GOTO
					return;
				end
				expected_mask <= get_expected_cond_jump_flags_mask((Opcode'((pkt.insn & 32'hff))));
				value1 <= create_vec_int64(expected_mask);
				value0.vec <= create_vec_int64((int32_t'((pkt.insn >> 8))));
				state <= 36; // GOTO
				return;
			end
		36:
			begin
				// CONTEXT_SWITCH();
				state <= 50; // GOTO
				return;
			end
		50:
			begin
				state <= 38; // GOTO
				return;
			end
		38:
			begin
				if (!(execute_bus.is_busy))
				begin
					state <= 51; // GOTO
					return;
				end
				state <= 36; // GOTO
				return;
			end
		51:
			begin
				state <= 37; // GOTO
				return;
			end
		37:
			begin
				execute_bus.send_req2(pkt.exec_mask, pkt.PC, EXEC_COND_JMP, value0, value1);
				state <= 35; // GOTO
				return;
				state <= 49; // GOTO
				return;
			end
		52:
			begin
				if (((Opcode'((pkt.insn & 32'hff))) == INSN_OPCODE_CMP_REG_REG))
				begin
				end
				else
				begin
					state <= 54; // GOTO
					return;
				end
				value0.vec <= regs.get((RegisterID'(((pkt.insn >> 8) & 32'hff))));
				value1 <= regs.get((RegisterID'(((pkt.insn >> 16) & 32'hff))));
				execute_bus.send_req2(pkt.exec_mask, pkt.PC, EXEC_CMP, value0, value1);
				state <= 35; // GOTO
				return;
				state <= 53; // GOTO
				return;
			end
		54:
			begin
				if (((Opcode'((pkt.insn & 32'hff))) == INSN_OPCODE_JMP_ALWAYS))
				begin
				end
				else
				begin
					state <= 56; // GOTO
					return;
				end
				value0.vec <= create_vec_int64((int32_t'((pkt.insn >> 8))));
				execute_bus.send_req1(pkt.exec_mask, pkt.PC, EXEC_JMP, value0);
				state <= 35; // GOTO
				return;
				state <= 55; // GOTO
				return;
			end
		56:
			begin
				if (((Opcode'((pkt.insn & 32'hff))) == INSN_OPCODE_ADD_REG_REG_CONST))
				begin
				end
				else
				begin
					state <= 58; // GOTO
					return;
				end
				value0.regID <= (RegisterID'(((pkt.insn >> 8) & 32'hff)));
				value2 <= create_vec_int64((int8_t'(((pkt.insn >> 24) & 32'hff))));
				value1 <= regs.get((RegisterID'(((pkt.insn >> 16) & 32'hff))));
				execute_bus.send_req3(pkt.exec_mask, pkt.PC, EXEC_ADD_REG_VALUE_VALUE, value0, value1, value2);
				state <= 35; // GOTO
				return;
				state <= 57; // GOTO
				return;
			end
		58:
			begin
				if (((Opcode'((pkt.insn & 32'hff))) == INSN_OPCODE_ADD_REG_REG_REG))
				begin
				end
				else
				begin
					state <= 60; // GOTO
					return;
				end
				value0.regID <= (RegisterID'(((pkt.insn >> 8) & 32'hff)));
				value1 <= regs.get((RegisterID'(((pkt.insn >> 16) & 32'hff))));
				value2 <= regs.get((RegisterID'(((pkt.insn >> 24) & 32'hff))));
				execute_bus.send_req3(pkt.exec_mask, pkt.PC, EXEC_ADD_REG_VALUE_VALUE, value0, value1, value2);
				state <= 35; // GOTO
				return;
				state <= 59; // GOTO
				return;
			end
		60:
			begin
				if (((Opcode'((pkt.insn & 32'hff))) == INSN_OPCODE_L_SSHIFT_REG_REG_CONST))
				begin
				end
				else
				begin
					state <= 62; // GOTO
					return;
				end
				value0.regID <= (RegisterID'(((pkt.insn >> 8) & 32'hff)));
				value1 <= regs.get((RegisterID'(((pkt.insn >> 16) & 32'hff))));
				value2 <= create_vec_int64((int8_t'(((pkt.insn >> 24) & 32'hff))));
				execute_bus.send_req3(pkt.exec_mask, pkt.PC, EXEC_SHL_REG_VALUE_VALUE, value0, value1, value2);
				state <= 35; // GOTO
				return;
				state <= 61; // GOTO
				return;
			end
		62:
			begin
				if (((Opcode'((pkt.insn & 32'hff))) == INSN_OPCODE_MOVE_REG_REG))
				begin
				end
				else
				begin
					state <= 64; // GOTO
					return;
				end
				value0.regID <= (RegisterID'(((pkt.insn >> 8) & 32'hff)));
				value1 <= regs.get((RegisterID'(((pkt.insn >> 16) & 32'hff))));
				execute_bus.send_req2(pkt.exec_mask, pkt.PC, EXEC_MOVE_REG_VALUE, value0, value1);
				state <= 35; // GOTO
				return;
				state <= 63; // GOTO
				return;
			end
		64:
			begin
				if (((Opcode'((pkt.insn & 32'hff))) == INSN_OPCODE_MOVE_REG_CONST16))
				begin
				end
				else
				begin
					state <= 66; // GOTO
					return;
				end
				value0.regID <= (RegisterID'(((pkt.insn >> 8) & 32'hff)));
				value1 <= create_vec_int64((int16_t'(((pkt.insn >> 16) & 32'hffff))));
				execute_bus.send_req2(pkt.exec_mask, pkt.PC, EXEC_MOVE_REG_VALUE, value0, value1);
				state <= 35; // GOTO
				return;
				state <= 65; // GOTO
				return;
			end
		66:
			begin
				if (((Opcode'((pkt.insn & 32'hff))) == INSN_OPCODE_MOVE_REG_BLOCK_INDEX))
				begin
				end
				else
				begin
					state <= 68; // GOTO
					return;
				end
				value0.regID <= (RegisterID'(((pkt.insn >> 8) & 32'hff)));
				value1 <= create_vec_incrementing_values();
				execute_bus.send_req2(pkt.exec_mask, pkt.PC, EXEC_MOVE_REG_VALUE, value0, value1);
				state <= 35; // GOTO
				return;
				state <= 67; // GOTO
				return;
			end
		68:
			begin
				if (((Opcode'((pkt.insn & 32'hff))) == INSN_OPCODE_STORE_REG_CONST_REG))
				begin
				end
				else
				begin
					state <= 70; // GOTO
					return;
				end
				value0.vec <= regs.get((RegisterID'(((pkt.insn >> 8) & 32'hff))));
				value1 <= create_vec_int64((int16_t'(((pkt.insn >> 24) & 32'hff))));
				value2 <= regs.get((RegisterID'(((pkt.insn >> 16) & 32'hff))));
				execute_bus.send_req3(pkt.exec_mask, pkt.PC, EXEC_STORE_ADDR_VALUE, value0, value1, value2);
				state <= 35; // GOTO
				return;
				state <= 69; // GOTO
				return;
			end
		70:
			begin
				if (((Opcode'((pkt.insn & 32'hff))) == INSN_OPCODE_MOVE_PCREL_REG_CONST16))
				begin
				end
				else
				begin
					state <= 72; // GOTO
					return;
				end
				value0.regID <= (RegisterID'(((pkt.insn >> 8) & 32'hff)));
				value1 <= create_vec_int64(((int64_t'(((pkt.insn >> 16) & 32'hffff))) + pkt.PC));
				execute_bus.send_req2(pkt.exec_mask, pkt.PC, EXEC_MOVE_REG_VALUE, value0, value1);
				state <= 35; // GOTO
				return;
				state <= 71; // GOTO
				return;
			end
		72:
			begin
				if (((Opcode'((pkt.insn & 32'hff))) == INSN_OPCODE_HALT))
				begin
				end
				else
				begin
					state <= 74; // GOTO
					return;
				end
				execute_bus.send_req1(pkt.exec_mask, pkt.PC, EXEC_HALT, value0);
				state <= 35; // GOTO
				return;
				state <= 73; // GOTO
				return;
			end
		74:
			begin
				if (((Opcode'((pkt.insn & 32'hff))) == INSN_OPCODE_NOP))
				begin
				end
				else
				begin
					state <= 76; // GOTO
					return;
				end
				state <= 35; // GOTO
				return;
				state <= 75; // GOTO
				return;
			end
		76:
			begin
				$display("[DECODE] unimplemented opcode: ", (Opcode'((pkt.insn & 32'hff))));
				assert(0);
				state <= 75; // GOTO
				return;
			end
		75:
			begin
				state <= 73; // GOTO
				return;
			end
		73:
			begin
				state <= 71; // GOTO
				return;
			end
		71:
			begin
				state <= 69; // GOTO
				return;
			end
		69:
			begin
				state <= 67; // GOTO
				return;
			end
		67:
			begin
				state <= 65; // GOTO
				return;
			end
		65:
			begin
				state <= 63; // GOTO
				return;
			end
		63:
			begin
				state <= 61; // GOTO
				return;
			end
		61:
			begin
				state <= 59; // GOTO
				return;
			end
		59:
			begin
				state <= 57; // GOTO
				return;
			end
		57:
			begin
				state <= 55; // GOTO
				return;
			end
		55:
			begin
				state <= 53; // GOTO
				return;
			end
		53:
			begin
				state <= 49; // GOTO
				return;
			end
		49:
			begin
				state <= 47; // GOTO
				return;
			end
		47:
			begin
				state <= 45; // GOTO
				return;
			end
		45:
			begin
				state <= 43; // GOTO
				return;
			end
		43:
			begin
				state <= 41; // GOTO
				return;
			end
		41:
			begin
				state <= 39; // GOTO
				return;
			end
		39:
			begin
				state <= 35; // GOTO
				return;
			end
		35:
			begin
				state <= 32; // GOTO
				return;
			end
		32:
			begin
				// CONTEXT_SWITCH();
				state <= 77; // GOTO
				return;
			end
		77:
			begin
				state <= 30; // GOTO
				return;
			end
		31:
			begin
			end
	endcase
	endtask
endmodule
