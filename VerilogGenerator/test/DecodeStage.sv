

function flags_reg_t get_expected_cond_jump_flags_mask(input Opcode opcode);
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
	flags_reg_t expected_mask;



	
	
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
				$display("DECODE exec: ", pkt.PC, (Opcode'(pkt.insn)));
				state <= 34; // GOTO
				return;
			end
		34:
			begin
				if (((Opcode'(pkt.insn)) == INSN_OPCODE_MOVE_R0_CONST24C))
				begin
				end
				else
				begin
					state <= 72; // GOTO
					return;
				end
				state <= 66; // GOTO
				return;
			end
		66:
			begin
				// CONTEXT_SWITCH();
				state <= 70; // GOTO
				return;
			end
		70:
			begin
				state <= 68; // GOTO
				return;
			end
		68:
			begin
				if (!(!(regs.is_valid(REG_R0))))
				begin
					state <= 71; // GOTO
					return;
				end
				state <= 66; // GOTO
				return;
			end
		71:
			begin
				state <= 67; // GOTO
				return;
			end
		67:
			begin
				value0.regID <= REG_R0;
				value1 <= create_vec_int32((int32_t'((pkt.insn >> 8))));
				value2 <= regs.get(REG_R0);
				execute_bus.send_req3(pkt.exec_mask, pkt.PC, EXEC_ORC_REG_VALUE, value0, value1, value2);
				state <= 35; // GOTO
				return;
				state <= 69; // GOTO
				return;
			end
		72:
			begin
				if (((Opcode'(pkt.insn)) == INSN_OPCODE_MOVE_R0_CONST24B))
				begin
				end
				else
				begin
					state <= 76; // GOTO
					return;
				end
				state <= 63; // GOTO
				return;
			end
		63:
			begin
				// CONTEXT_SWITCH();
				state <= 74; // GOTO
				return;
			end
		74:
			begin
				state <= 65; // GOTO
				return;
			end
		65:
			begin
				if (!(!(regs.is_valid(REG_R0))))
				begin
					state <= 75; // GOTO
					return;
				end
				state <= 63; // GOTO
				return;
			end
		75:
			begin
				state <= 64; // GOTO
				return;
			end
		64:
			begin
				value0.regID <= REG_R0;
				value1 <= create_vec_int32((int32_t'((pkt.insn >> 8))));
				value2 <= regs.get(REG_R0);
				execute_bus.send_req3(pkt.exec_mask, pkt.PC, EXEC_ORB_REG_VALUE, value0, value1, value2);
				state <= 35; // GOTO
				return;
				state <= 73; // GOTO
				return;
			end
		76:
			begin
				if (((Opcode'(pkt.insn)) == INSN_OPCODE_MOVE_R0_CONST24A))
				begin
				end
				else
				begin
					state <= 78; // GOTO
					return;
				end
				value0.regID <= REG_R0;
				value1 <= create_vec_int32((int32_t'((pkt.insn >> 8))));
				execute_bus.send_req2(pkt.exec_mask, pkt.PC, EXEC_MOVE_REG_VALUE, value0, value1);
				state <= 35; // GOTO
				return;
				state <= 77; // GOTO
				return;
			end
		78:
			begin
				if (((Opcode'(pkt.insn)) == INSN_OPCODE_LOAD_REG_CONST_REG))
				begin
				end
				else
				begin
					state <= 82; // GOTO
					return;
				end
				state <= 60; // GOTO
				return;
			end
		60:
			begin
				// CONTEXT_SWITCH();
				state <= 80; // GOTO
				return;
			end
		80:
			begin
				state <= 62; // GOTO
				return;
			end
		62:
			begin
				if (!(!(regs.is_valid((RegisterID'((pkt.insn >> 16)))))))
				begin
					state <= 81; // GOTO
					return;
				end
				state <= 60; // GOTO
				return;
			end
		81:
			begin
				state <= 61; // GOTO
				return;
			end
		61:
			begin
				value0.regID <= (RegisterID'((pkt.insn >> 8)));
				value2 <= regs.get((RegisterID'((pkt.insn >> 16))));
				value1 <= create_vec_int16((int16_t'((pkt.insn >> 24))));
				execute_bus.send_req3(pkt.exec_mask, pkt.PC, EXEC_LOAD_REG, value0, value1, value2);
				state <= 35; // GOTO
				return;
				state <= 79; // GOTO
				return;
			end
		82:
			begin
				if (((Opcode'(pkt.insn)) == INSN_OPCODE_LOAD_RESTORE_PC))
				begin
				end
				else
				begin
					state <= 86; // GOTO
					return;
				end
				state <= 57; // GOTO
				return;
			end
		57:
			begin
				// CONTEXT_SWITCH();
				state <= 84; // GOTO
				return;
			end
		84:
			begin
				state <= 59; // GOTO
				return;
			end
		59:
			begin
				if (!(!(regs.is_valid((RegisterID'((pkt.insn >> 8)))))))
				begin
					state <= 85; // GOTO
					return;
				end
				state <= 57; // GOTO
				return;
			end
		85:
			begin
				state <= 58; // GOTO
				return;
			end
		58:
			begin
				value0.vec <= create_vec_int16((int16_t'((pkt.insn >> 16))));
				value1 <= regs.get((RegisterID'((pkt.insn >> 8))));
				execute_bus.send_req2(pkt.exec_mask, pkt.PC, EXEC_LOAD_RESTORE_PC, value0, value1);
				state <= 35; // GOTO
				return;
				state <= 83; // GOTO
				return;
			end
		86:
			begin
				if ((((((((Opcode'(pkt.insn)) == INSN_OPCODE_JMP_LOWER) || ((Opcode'(pkt.insn)) == INSN_OPCODE_JMP_LOWER_EQUAL)) || ((Opcode'(pkt.insn)) == INSN_OPCODE_JMP_EQUAL)) || ((Opcode'(pkt.insn)) == INSN_OPCODE_JMP_NOT_EQUAL)) || ((Opcode'(pkt.insn)) == INSN_OPCODE_JMP_GREATER)) || ((Opcode'(pkt.insn)) == INSN_OPCODE_JMP_GREATER_EQUAL)))
				begin
				end
				else
				begin
					state <= 90; // GOTO
					return;
				end
				expected_mask <= get_expected_cond_jump_flags_mask((Opcode'(pkt.insn)));
				value1 <= create_vec_int64(expected_mask);
				value0.vec <= create_vec_int32((int32_t'((pkt.insn >> 8))));
				state <= 54; // GOTO
				return;
			end
		54:
			begin
				// CONTEXT_SWITCH();
				state <= 88; // GOTO
				return;
			end
		88:
			begin
				state <= 56; // GOTO
				return;
			end
		56:
			begin
				if (!(execute_bus.is_busy))
				begin
					state <= 89; // GOTO
					return;
				end
				state <= 54; // GOTO
				return;
			end
		89:
			begin
				state <= 55; // GOTO
				return;
			end
		55:
			begin
				execute_bus.send_req2(pkt.exec_mask, pkt.PC, EXEC_COND_JMP, value0, value1);
				state <= 35; // GOTO
				return;
				state <= 87; // GOTO
				return;
			end
		90:
			begin
				if (((Opcode'(pkt.insn)) == INSN_OPCODE_CMP_REG_REG))
				begin
				end
				else
				begin
					state <= 94; // GOTO
					return;
				end
				state <= 51; // GOTO
				return;
			end
		51:
			begin
				// CONTEXT_SWITCH();
				state <= 92; // GOTO
				return;
			end
		92:
			begin
				state <= 53; // GOTO
				return;
			end
		53:
			begin
				if (!((!(regs.is_valid((RegisterID'((pkt.insn >> 8))))) | !(regs.is_valid((RegisterID'((pkt.insn >> 16))))))))
				begin
					state <= 93; // GOTO
					return;
				end
				state <= 51; // GOTO
				return;
			end
		93:
			begin
				state <= 52; // GOTO
				return;
			end
		52:
			begin
				value0.vec <= regs.get((RegisterID'((pkt.insn >> 8))));
				value1 <= regs.get((RegisterID'((pkt.insn >> 16))));
				execute_bus.send_req2(pkt.exec_mask, pkt.PC, EXEC_CMP, value0, value1);
				state <= 35; // GOTO
				return;
				state <= 91; // GOTO
				return;
			end
		94:
			begin
				if (((Opcode'(pkt.insn)) == INSN_OPCODE_JMP_ALWAYS))
				begin
				end
				else
				begin
					state <= 96; // GOTO
					return;
				end
				value0.vec <= create_vec_int32((int32_t'((pkt.insn >> 8))));
				execute_bus.send_req1(pkt.exec_mask, pkt.PC, EXEC_JMP, value0);
				state <= 35; // GOTO
				return;
				state <= 95; // GOTO
				return;
			end
		96:
			begin
				if (((Opcode'(pkt.insn)) == INSN_OPCODE_ADD_REG_REG_CONST))
				begin
				end
				else
				begin
					state <= 100; // GOTO
					return;
				end
				state <= 48; // GOTO
				return;
			end
		48:
			begin
				// CONTEXT_SWITCH();
				state <= 98; // GOTO
				return;
			end
		98:
			begin
				state <= 50; // GOTO
				return;
			end
		50:
			begin
				if (!(!(regs.is_valid((RegisterID'((pkt.insn >> 16)))))))
				begin
					state <= 99; // GOTO
					return;
				end
				state <= 48; // GOTO
				return;
			end
		99:
			begin
				state <= 49; // GOTO
				return;
			end
		49:
			begin
				value0.regID <= (RegisterID'((pkt.insn >> 8)));
				value2 <= create_vec_int8((int8_t'((pkt.insn >> 24))));
				value1 <= regs.get((RegisterID'((pkt.insn >> 16))));
				execute_bus.send_req3(pkt.exec_mask, pkt.PC, EXEC_ADD_REG_VALUE_VALUE, value0, value1, value2);
				state <= 35; // GOTO
				return;
				state <= 97; // GOTO
				return;
			end
		100:
			begin
				if (((Opcode'(pkt.insn)) == INSN_OPCODE_ADD_REG_REG_REG))
				begin
				end
				else
				begin
					state <= 104; // GOTO
					return;
				end
				state <= 45; // GOTO
				return;
			end
		45:
			begin
				// CONTEXT_SWITCH();
				state <= 102; // GOTO
				return;
			end
		102:
			begin
				state <= 47; // GOTO
				return;
			end
		47:
			begin
				if (!((!(regs.is_valid((RegisterID'((pkt.insn >> 16))))) | !(regs.is_valid((RegisterID'((pkt.insn >> 24))))))))
				begin
					state <= 103; // GOTO
					return;
				end
				state <= 45; // GOTO
				return;
			end
		103:
			begin
				state <= 46; // GOTO
				return;
			end
		46:
			begin
				value0.regID <= (RegisterID'((pkt.insn >> 8)));
				value1 <= regs.get((RegisterID'((pkt.insn >> 16))));
				value2 <= regs.get((RegisterID'((pkt.insn >> 24))));
				execute_bus.send_req3(pkt.exec_mask, pkt.PC, EXEC_ADD_REG_VALUE_VALUE, value0, value1, value2);
				state <= 35; // GOTO
				return;
				state <= 101; // GOTO
				return;
			end
		104:
			begin
				if (((Opcode'(pkt.insn)) == INSN_OPCODE_L_SSHIFT_REG_REG_CONST))
				begin
				end
				else
				begin
					state <= 108; // GOTO
					return;
				end
				state <= 42; // GOTO
				return;
			end
		42:
			begin
				// CONTEXT_SWITCH();
				state <= 106; // GOTO
				return;
			end
		106:
			begin
				state <= 44; // GOTO
				return;
			end
		44:
			begin
				if (!(!(regs.is_valid((RegisterID'((pkt.insn >> 16)))))))
				begin
					state <= 107; // GOTO
					return;
				end
				state <= 42; // GOTO
				return;
			end
		107:
			begin
				state <= 43; // GOTO
				return;
			end
		43:
			begin
				value0.regID <= (RegisterID'((pkt.insn >> 8)));
				value1 <= regs.get((RegisterID'((pkt.insn >> 16))));
				value2 <= create_vec_int8((int8_t'((pkt.insn >> 24))));
				execute_bus.send_req3(pkt.exec_mask, pkt.PC, EXEC_SHL_REG_VALUE_VALUE, value0, value1, value2);
				state <= 35; // GOTO
				return;
				state <= 105; // GOTO
				return;
			end
		108:
			begin
				if (((Opcode'(pkt.insn)) == INSN_OPCODE_MOVE_REG_REG))
				begin
				end
				else
				begin
					state <= 112; // GOTO
					return;
				end
				state <= 39; // GOTO
				return;
			end
		39:
			begin
				// CONTEXT_SWITCH();
				state <= 110; // GOTO
				return;
			end
		110:
			begin
				state <= 41; // GOTO
				return;
			end
		41:
			begin
				if (!(!(regs.is_valid((RegisterID'((pkt.insn >> 16)))))))
				begin
					state <= 111; // GOTO
					return;
				end
				state <= 39; // GOTO
				return;
			end
		111:
			begin
				state <= 40; // GOTO
				return;
			end
		40:
			begin
				value0.regID <= (RegisterID'((pkt.insn >> 8)));
				value1 <= regs.get((RegisterID'((pkt.insn >> 16))));
				execute_bus.send_req2(pkt.exec_mask, pkt.PC, EXEC_MOVE_REG_VALUE, value0, value1);
				state <= 35; // GOTO
				return;
				state <= 109; // GOTO
				return;
			end
		112:
			begin
				if (((Opcode'(pkt.insn)) == INSN_OPCODE_MOVE_REG_CONST16))
				begin
				end
				else
				begin
					state <= 114; // GOTO
					return;
				end
				value0.regID <= (RegisterID'((pkt.insn >> 8)));
				value1 <= create_vec_int16((int16_t'((pkt.insn >> 16))));
				execute_bus.send_req2(pkt.exec_mask, pkt.PC, EXEC_MOVE_REG_VALUE, value0, value1);
				state <= 35; // GOTO
				return;
				state <= 113; // GOTO
				return;
			end
		114:
			begin
				if (((Opcode'(pkt.insn)) == INSN_OPCODE_MOVE_REG_BLOCK_INDEX))
				begin
				end
				else
				begin
					state <= 116; // GOTO
					return;
				end
				value0.regID <= (RegisterID'((pkt.insn >> 8)));
				value1 <= create_vec_incrementing_values();
				execute_bus.send_req2(pkt.exec_mask, pkt.PC, EXEC_MOVE_REG_VALUE, value0, value1);
				state <= 35; // GOTO
				return;
				state <= 115; // GOTO
				return;
			end
		116:
			begin
				if (((Opcode'(pkt.insn)) == INSN_OPCODE_STORE_REG_CONST_REG))
				begin
				end
				else
				begin
					state <= 120; // GOTO
					return;
				end
				state <= 36; // GOTO
				return;
			end
		36:
			begin
				// CONTEXT_SWITCH();
				state <= 118; // GOTO
				return;
			end
		118:
			begin
				state <= 38; // GOTO
				return;
			end
		38:
			begin
				if (!((!(regs.is_valid((RegisterID'((pkt.insn >> 8))))) | !(regs.is_valid((RegisterID'((pkt.insn >> 16))))))))
				begin
					state <= 119; // GOTO
					return;
				end
				state <= 36; // GOTO
				return;
			end
		119:
			begin
				state <= 37; // GOTO
				return;
			end
		37:
			begin
				value0.vec <= regs.get((RegisterID'((pkt.insn >> 8))));
				value2 <= regs.get((RegisterID'((pkt.insn >> 16))));
				value1 <= create_vec_int16((int16_t'((uint8_t'((pkt.insn >> 24))))));
				execute_bus.send_req3(pkt.exec_mask, pkt.PC, EXEC_STORE_ADDR_VALUE, value0, value1, value2);
				state <= 35; // GOTO
				return;
				state <= 117; // GOTO
				return;
			end
		120:
			begin
				if (((Opcode'(pkt.insn)) == INSN_OPCODE_MOVE_PCREL_REG_CONST16))
				begin
				end
				else
				begin
					state <= 122; // GOTO
					return;
				end
				value0.regID <= (RegisterID'((pkt.insn >> 8)));
				value1 <= create_vec_int64(((int64_t'((uint16_t'((pkt.insn >> 16))))) + pkt.PC));
				execute_bus.send_req2(pkt.exec_mask, pkt.PC, EXEC_MOVE_REG_VALUE, value0, value1);
				state <= 35; // GOTO
				return;
				state <= 121; // GOTO
				return;
			end
		122:
			begin
				if (((Opcode'(pkt.insn)) == INSN_OPCODE_HALT))
				begin
				end
				else
				begin
					state <= 124; // GOTO
					return;
				end
				execute_bus.send_req1(pkt.exec_mask, pkt.PC, EXEC_HALT, value0);
				state <= 35; // GOTO
				return;
				state <= 123; // GOTO
				return;
			end
		124:
			begin
				if (((Opcode'(pkt.insn)) == INSN_OPCODE_NOP))
				begin
				end
				else
				begin
					state <= 126; // GOTO
					return;
				end
				state <= 35; // GOTO
				return;
				state <= 125; // GOTO
				return;
			end
		126:
			begin
				$display("[DECODE] unimplemented opcode: ", (Opcode'(pkt.insn)));
				assert(0);
				state <= 125; // GOTO
				return;
			end
		125:
			begin
				state <= 123; // GOTO
				return;
			end
		123:
			begin
				state <= 121; // GOTO
				return;
			end
		121:
			begin
				state <= 117; // GOTO
				return;
			end
		117:
			begin
				state <= 115; // GOTO
				return;
			end
		115:
			begin
				state <= 113; // GOTO
				return;
			end
		113:
			begin
				state <= 109; // GOTO
				return;
			end
		109:
			begin
				state <= 105; // GOTO
				return;
			end
		105:
			begin
				state <= 101; // GOTO
				return;
			end
		101:
			begin
				state <= 97; // GOTO
				return;
			end
		97:
			begin
				state <= 95; // GOTO
				return;
			end
		95:
			begin
				state <= 91; // GOTO
				return;
			end
		91:
			begin
				state <= 87; // GOTO
				return;
			end
		87:
			begin
				state <= 83; // GOTO
				return;
			end
		83:
			begin
				state <= 79; // GOTO
				return;
			end
		79:
			begin
				state <= 77; // GOTO
				return;
			end
		77:
			begin
				state <= 73; // GOTO
				return;
			end
		73:
			begin
				state <= 69; // GOTO
				return;
			end
		69:
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
				state <= 127; // GOTO
				return;
			end
		127:
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
