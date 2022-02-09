#include "CoreLib.h"
#include "MachineInfo.h"
#include <cassert>

namespace ecc
{

	uint64_t get_expected_cond_jump_flags_mask(Opcode opcode)
	{
		switch (opcode)
		{
		case Opcode::INSN_OPCODE_JMP_EQUAL:		return FLAGS_MASK_EQ;
		case Opcode::INSN_OPCODE_JMP_NOT_EQUAL:	return FLAGS_MASK_LT | FLAGS_MASK_GT;

		case Opcode::INSN_OPCODE_JMP_GREATER:	return FLAGS_MASK_GT;
		case Opcode::INSN_OPCODE_JMP_LOWER:		return FLAGS_MASK_LT;

		case Opcode::INSN_OPCODE_JMP_GREATER_EQUAL:	return FLAGS_MASK_GT | FLAGS_MASK_EQ;
		case Opcode::INSN_OPCODE_JMP_LOWER_EQUAL:	return FLAGS_MASK_LT | FLAGS_MASK_EQ;
		}
		assert(false);
		return FLAGS_MASK_GT;
	}

	ReturnObject DecodeStage::run()
	{
		while (1)
		{
			if (fetch_bus.is_busy)
			{
				const FetchToDecodeBusPacket pkt = fetch_bus.recv();

				auto PC = pkt.PC;
				auto opcode = (Opcode)(pkt.insn & 0xff);

				logger.debug("DECODE[" + std::to_string(PC) + "] exec: " + to_string(opcode));

				switch (opcode)
				{
				case Opcode::INSN_OPCODE_NOP:
				{
					break;
				}
				case Opcode::INSN_OPCODE_HALT:
				{
					DecodeToExecuteBus::Packet execute_pkt{ pkt.exec_mask, PC, ExecuteStageOpcode::EXEC_HALT };
					execute_bus.send(execute_pkt);
					break;
				}

				case Opcode::INSN_OPCODE_MOVE_PCREL_REG_CONST16:
				{
					auto reg = static_cast<RegisterID>((pkt.insn >> 8) & 0xff);
					auto constValue = static_cast<int64_t>((pkt.insn >> 16) & 0xffff);

					memory_address_t value = constValue + pkt.PC;

					VectorValue vec = create_vec_int64(value);

					DecodeToExecuteBus::Packet execute_pkt{ pkt.exec_mask, PC, ExecuteStageOpcode::EXEC_MOVE_REG_VALUE,
									reg,
									vec,
					};
					execute_bus.send(execute_pkt);
					break;
				}

				// reg = [reg + const]
				case Opcode::INSN_OPCODE_STORE_REG_CONST_REG:
				{
					const auto& src_reg = static_cast<RegisterID>((pkt.insn >> 8) & 0xff);
					const auto& base_reg_id = static_cast<RegisterID>((pkt.insn >> 16) & 0xff);
					const auto& addr_offset_value = static_cast<int16_t>((pkt.insn >> 24) & 0xff);
					const auto& addr2 = regs[base_reg_id];
					const auto& value = regs[src_reg];

					VectorValue addr1 = create_vec_int64(addr_offset_value);

					DecodeToExecuteBus::Packet execute_pkt{ pkt.exec_mask, PC, ExecuteStageOpcode::EXEC_STORE_ADDR_VALUE,
						value, addr1, addr2 };
					execute_bus.send(execute_pkt);
					break;
				}

				// reg = blockIndex
				case Opcode::INSN_OPCODE_MOVE_REG_BLOCK_INDEX:
				{
					const auto& reg = static_cast<RegisterID>((pkt.insn >> 8) & 0xff);

					VectorValue value = create_vec_int64_blockindex();

					DecodeToExecuteBus::Packet execute_pkt{ pkt.exec_mask, PC, ExecuteStageOpcode::EXEC_MOVE_REG_VALUE,
						reg, value };
					execute_bus.send(execute_pkt);
					break;
				}

				case Opcode::INSN_OPCODE_MOVE_REG_CONST16:
				{
					const auto& reg = static_cast<RegisterID>((pkt.insn >> 8) & 0xff);
					const auto& const_value = static_cast<int16_t>((pkt.insn >> 16) & 0xffff);

					VectorValue value = create_vec_int64(const_value);

					DecodeToExecuteBus::Packet execute_pkt{ pkt.exec_mask,  PC, ExecuteStageOpcode::EXEC_MOVE_REG_VALUE,
						reg, value };
					execute_bus.send(execute_pkt);
					break;
				}

				case Opcode::INSN_OPCODE_MOVE_REG_REG:
				{
					const auto reg = static_cast<RegisterID>((pkt.insn >> 8) & 0xff);
					const auto src_reg = static_cast<RegisterID>((pkt.insn >> 16) & 0xff);

					const auto& value = regs[src_reg];

					DecodeToExecuteBus::Packet execute_pkt{ pkt.exec_mask, PC, ExecuteStageOpcode::EXEC_MOVE_REG_VALUE,
						reg, value };
					execute_bus.send(execute_pkt);
					break;
				}

				case Opcode::INSN_OPCODE_L_SSHIFT_REG_REG_CONST:
				{
					const auto dst_reg = static_cast<RegisterID>((pkt.insn >> 8) & 0xff);
					const auto src1_reg = static_cast<RegisterID>((pkt.insn >> 16) & 0xff);
					const auto value2_const = static_cast<int8_t>((pkt.insn >> 24) & 0xff);

					const auto value2 = create_vec_int64(value2_const);

					const auto& value1 = regs[src1_reg];

					DecodeToExecuteBus::Packet execute_pkt{ pkt.exec_mask, PC, ExecuteStageOpcode::EXEC_SHL_REG_VALUE_VALUE,
						dst_reg, value1, value2 };
					execute_bus.send(execute_pkt);
					break;
				}


				case Opcode::INSN_OPCODE_ADD_REG_REG_REG:
				{
					const auto dst_reg = static_cast<RegisterID>((pkt.insn >> 8) & 0xff);
					const auto src1_reg = static_cast<RegisterID>((pkt.insn >> 16) & 0xff);
					const auto src2_reg = static_cast<RegisterID>((pkt.insn >> 24) & 0xff);

					const auto& value1 = regs[src1_reg];
					const auto& value2 = regs[src2_reg];

					DecodeToExecuteBus::Packet execute_pkt{ pkt.exec_mask, PC, ExecuteStageOpcode::EXEC_ADD_REG_VALUE_VALUE,
						dst_reg, value1, value2 };
					execute_bus.send(execute_pkt);
					break;
				}


				// reg = reg + const
				case Opcode::INSN_OPCODE_ADD_REG_REG_CONST:
				{
					const auto dst_reg = static_cast<RegisterID>((pkt.insn >> 8) & 0xff);
					const auto src1_reg = static_cast<RegisterID>((pkt.insn >> 16) & 0xff);
					const auto value2_const = static_cast<int8_t>((pkt.insn >> 24) & 0xff);

					VectorValue value2 = create_vec_int64(value2_const);

					const auto& value1 = regs[src1_reg];

					DecodeToExecuteBus::Packet execute_pkt{ pkt.exec_mask, PC, ExecuteStageOpcode::EXEC_ADD_REG_VALUE_VALUE,
						dst_reg, value1, value2 };
					execute_bus.send(execute_pkt);
					break;
				}

				case Opcode::INSN_OPCODE_JMP_ALWAYS:
				{
					const auto off = static_cast<int32_t>(pkt.insn >> 8);

					VectorValue v;
					v.setPC(off);

					DecodeToExecuteBus::Packet execute_pkt{ pkt.exec_mask, PC, ExecuteStageOpcode::EXEC_JMP,
						v };
					execute_bus.send(execute_pkt);
					break;
				}

				case Opcode::INSN_OPCODE_CMP_REG_REG:
				{
					const auto reg1 = static_cast<RegisterID>((pkt.insn >> 8) & 0xff);
					const auto reg2 = static_cast<RegisterID>((pkt.insn >> 16) & 0xff);

					const auto& value1 = regs[reg1];
					const auto& value2 = regs[reg2];

					//std::cerr << "[DECODE] CMP: " << value1 << " -- " << value2 << std::endl;

					DecodeToExecuteBus::Packet execute_pkt{ pkt.exec_mask, PC, ExecuteStageOpcode::EXEC_CMP,
						value1,
						value2,
					};
					execute_bus.send(execute_pkt);
					break;
				}

				case Opcode::INSN_OPCODE_JMP_LOWER:
				case Opcode::INSN_OPCODE_JMP_LOWER_EQUAL:
				case Opcode::INSN_OPCODE_JMP_EQUAL:
				case Opcode::INSN_OPCODE_JMP_NOT_EQUAL:
				case Opcode::INSN_OPCODE_JMP_GREATER:
				case Opcode::INSN_OPCODE_JMP_GREATER_EQUAL:
				{
					const auto PC = pkt.PC;

					const auto expected_mask = get_expected_cond_jump_flags_mask(opcode);

					const VectorValue jmp_mask = create_vec_int64(expected_mask);

					const auto off_const = static_cast<int32_t>(pkt.insn >> 8);
					const VectorValue off = create_vec_int64(off_const);


					const DecodeToExecuteBus::Packet execute_pkt{ pkt.exec_mask, PC,
						ExecuteStageOpcode::EXEC_COND_JMP,
						off, jmp_mask };
					while (execute_bus.is_busy)
					{
						CONTEXT_SWITCH();
					}
					execute_bus.send(execute_pkt);
					break;
				}


				case Opcode::INSN_OPCODE_LOAD_RESTORE_PC:
				{
					const auto base = static_cast<RegisterID>((pkt.insn >> 8) & 0xff);
					const auto off1_const = static_cast<int16_t>(pkt.insn >> 16);

					VectorValue off1 = create_vec_int64(off1_const);

					const auto& off2 = regs[base];

					DecodeToExecuteBus::Packet execute_pkt{ pkt.exec_mask, PC, ExecuteStageOpcode::EXEC_LOAD_RESTORE_PC,
							off1,
							off2,
					};
					execute_bus.send(execute_pkt);
					break;
				}

				// ret = [const + reg]
				case Opcode::INSN_OPCODE_LOAD_REG_CONST_REG:
				{
					const auto dest = static_cast<RegisterID>((pkt.insn >> 8) & 0xff);
					const auto base = static_cast<RegisterID>((pkt.insn >> 16) & 0xff);
					const auto off1_const = static_cast<int16_t>(pkt.insn >> 24);

					VectorValue off1 = create_vec_int64(off1_const);

					const auto& off2 = regs[base];

					DecodeToExecuteBus::Packet execute_pkt{ pkt.exec_mask, PC, ExecuteStageOpcode::EXEC_LOAD_REG,
							dest,
							off1,
							off2,
					};
					execute_bus.send(execute_pkt);
					break;
				}

				case Opcode::INSN_OPCODE_MOVE_R0_CONST24A:
				{
					const auto dest = RegisterID::REG_R0;
					const auto off1_const = static_cast<int32_t>(pkt.insn >> 8);
					VectorValue off1 = create_vec_int64(off1_const);

					DecodeToExecuteBus::Packet execute_pkt{ pkt.exec_mask, PC, ExecuteStageOpcode::EXEC_MOVE_REG_VALUE,
							dest,
							off1
					};
					execute_bus.send(execute_pkt);
					break;
				}

				case Opcode::INSN_OPCODE_MOVE_R0_CONST24B:
				{
					const auto dest = RegisterID::REG_R0;
					const auto off1_const = static_cast<int32_t>(pkt.insn >> 8);
					VectorValue off1 = create_vec_int64(off1_const);
					const auto& off2 = regs[RegisterID::REG_R0];

					DecodeToExecuteBus::Packet execute_pkt{ pkt.exec_mask, PC, ExecuteStageOpcode::EXEC_ORB_REG_VALUE,
							dest,
							off1,
							off2
					};
					execute_bus.send(execute_pkt);
					break;
				}

				case Opcode::INSN_OPCODE_MOVE_R0_CONST24C:
				{
					const auto dest = RegisterID::REG_R0;
					const auto off1_const = static_cast<int32_t>(pkt.insn >> 8);
					VectorValue off1 = create_vec_int64(off1_const);
					const auto& off2 = regs[RegisterID::REG_R0];

					DecodeToExecuteBus::Packet execute_pkt{ pkt.exec_mask, PC, ExecuteStageOpcode::EXEC_ORC_REG_VALUE,
							dest,
							off1,
							off2
					};
					execute_bus.send(execute_pkt);
					break;
				}

				default:
					logger.debug("[DECODE] unimplemented opcode: " + to_string(opcode));
					abort();
				}
			}


			CONTEXT_SWITCH();
		}
	}

}