#include "CoreLib.h"
#include "MachineInfo.h"


namespace Simulator
{
	coro::ReturnObject DecodeStage::run()
	{
		while (1)
		{
			if (auto pkt_opt = this->fetch_bus.try_recv())
			{
				const auto& pkt = *pkt_opt;

				auto PC = pkt.PC;
				auto opcode = (MachineInfo::Opcode)(pkt.insn & 0xff);

				logger.debug("DECODE[" + std::to_string(PC) + "] exec: " + MachineInfo::to_string(opcode));

				switch (opcode)
				{
				case MachineInfo::Opcode::NOP:
				{
					break;
				}
				case MachineInfo::Opcode::HALT:
				{
					DecodeToExecuteBus::Packet execute_pkt{ PC, MachineInfo::ExecuteStageOpcode::HALT
					};
					execute_bus.send(execute_pkt);
					break;
				}

				case MachineInfo::Opcode::MOVE_PCREL_REG_CONST16:
				{
					auto reg = static_cast<MachineInfo::RegisterID>((pkt.insn >> 8) & 0xff);
					auto constValue = static_cast<int64_t>((pkt.insn >> 16) & 0xffff);

					MachineInfo::memory_address_t value = constValue + pkt.PC;

					VectorValue vec = VectorValue::create_vec_int64(value);

					DecodeToExecuteBus::Packet execute_pkt{ PC, MachineInfo::ExecuteStageOpcode::MOVE_REG_VALUE,
									reg,
									vec,
					};
					execute_bus.send(execute_pkt);
					break;
				}

				// reg = [reg + const]
				case MachineInfo::Opcode::STORE_REG_CONST_REG:
				{
					const auto& src_reg = static_cast<MachineInfo::RegisterID>((pkt.insn >> 8) & 0xff);
					const auto& base_reg_id = static_cast<MachineInfo::RegisterID>((pkt.insn >> 16) & 0xff);
					const auto& addr_offset_value = static_cast<int16_t>((pkt.insn >> 24) & 0xff);
					const auto& addr2 = regs[base_reg_id];
					const auto& value = regs[src_reg];

					VectorValue addr1 = VectorValue::create_vec_int64(addr_offset_value);

					DecodeToExecuteBus::Packet execute_pkt{ PC, MachineInfo::ExecuteStageOpcode::STORE_ADDR_VALUE,
						value, addr1, addr2 };
					execute_bus.send(execute_pkt);
					break;
				}

				// reg = blockIndex
				case MachineInfo::Opcode::MOVE_REG_BLOCK_INDEX:
				{
					const auto& reg = static_cast<MachineInfo::RegisterID>((pkt.insn >> 8) & 0xff);

					VectorValue value = VectorValue::create_vec_int64_blockindex();

					DecodeToExecuteBus::Packet execute_pkt{ PC, MachineInfo::ExecuteStageOpcode::MOVE_REG_VALUE,
						reg, value };
					execute_bus.send(execute_pkt);
					break;
				}

				case MachineInfo::Opcode::MOVE_REG_CONST16:
				{
					const auto& reg = static_cast<MachineInfo::RegisterID>((pkt.insn >> 8) & 0xff);
					const auto& const_value = static_cast<int16_t>((pkt.insn >> 16) & 0xffff);

					VectorValue value = VectorValue::create_vec_int64(const_value);

					DecodeToExecuteBus::Packet execute_pkt{ PC, MachineInfo::ExecuteStageOpcode::MOVE_REG_VALUE,
						reg, value };
					execute_bus.send(execute_pkt);
					break;
				}

				case MachineInfo::Opcode::MOVE_REG_REG:
				{
					const auto reg = static_cast<MachineInfo::RegisterID>((pkt.insn >> 8) & 0xff);
					const auto src_reg = static_cast<MachineInfo::RegisterID>((pkt.insn >> 16) & 0xff);

					const auto& value = regs[src_reg];

					DecodeToExecuteBus::Packet execute_pkt{ PC, MachineInfo::ExecuteStageOpcode::MOVE_REG_VALUE,
						reg, value };
					execute_bus.send(execute_pkt);
					break;
				}

				case MachineInfo::Opcode::L_SSHIFT_REG_REG_CONST:
				{
					const auto dst_reg = static_cast<MachineInfo::RegisterID>((pkt.insn >> 8) & 0xff);
					const auto src1_reg = static_cast<MachineInfo::RegisterID>((pkt.insn >> 16) & 0xff);
					const auto value2_const = static_cast<int8_t>((pkt.insn >> 24) & 0xff);

					const auto value2 = VectorValue::create_vec_int64(value2_const);

					const auto& value1 = regs[src1_reg];

					DecodeToExecuteBus::Packet execute_pkt{ PC, MachineInfo::ExecuteStageOpcode::SHL_REG_VALUE_VALUE,
						dst_reg, value1, value2 };
					execute_bus.send(execute_pkt);
					break;
				}


				case MachineInfo::Opcode::ADD_REG_REG_REG:
				{
					const auto dst_reg = static_cast<MachineInfo::RegisterID>((pkt.insn >> 8) & 0xff);
					const auto src1_reg = static_cast<MachineInfo::RegisterID>((pkt.insn >> 16) & 0xff);
					const auto src2_reg = static_cast<MachineInfo::RegisterID>((pkt.insn >> 24) & 0xff);

					const auto& value1 = regs[src1_reg];
					const auto& value2 = regs[src2_reg];

					DecodeToExecuteBus::Packet execute_pkt{ PC, MachineInfo::ExecuteStageOpcode::ADD_REG_VALUE_VALUE,
						dst_reg, value1, value2 };
					execute_bus.send(execute_pkt);
					break;
				}


				// reg = reg + const
				case MachineInfo::Opcode::ADD_REG_REG_CONST:
				{
					const auto dst_reg = static_cast<MachineInfo::RegisterID>((pkt.insn >> 8) & 0xff);
					const auto src1_reg = static_cast<MachineInfo::RegisterID>((pkt.insn >> 16) & 0xff);
					const auto value2_const = static_cast<int8_t>((pkt.insn >> 24) & 0xff);

					VectorValue value2 = VectorValue::create_vec_int64(value2_const);

					const auto& value1 = regs[src1_reg];

					DecodeToExecuteBus::Packet execute_pkt{ PC, MachineInfo::ExecuteStageOpcode::ADD_REG_VALUE_VALUE,
						dst_reg, value1, value2 };
					execute_bus.send(execute_pkt);
					break;
				}

				case MachineInfo::Opcode::JMP_ALWAYS:
				{
					const auto off = static_cast<int32_t>(pkt.insn >> 8);

					VectorValue v;
					v.setPC(off);

					DecodeToExecuteBus::Packet execute_pkt{ PC, MachineInfo::ExecuteStageOpcode::JMP,
						v };
					execute_bus.send(execute_pkt);
					break;
				}

				case MachineInfo::Opcode::CMP_REG_REG:
				{
					const auto reg1 = static_cast<MachineInfo::RegisterID>((pkt.insn >> 8) & 0xff);
					const auto reg2 = static_cast<MachineInfo::RegisterID>((pkt.insn >> 16) & 0xff);

					const auto& value1 = regs[reg1];
					const auto& value2 = regs[reg2];

					//std::cerr << "[DECODE] CMP: " << value1 << " -- " << value2 << std::endl;

					DecodeToExecuteBus::Packet execute_pkt{ PC, MachineInfo::ExecuteStageOpcode::CMP,
						value1,
						value2,
					};
					execute_bus.send(execute_pkt);
					break;
				}


				case MachineInfo::Opcode::JMP_EQUAL:
				{
					handle_conditional_jump(MachineInfo::FLAGS_MASK_EQ, pkt);
					break;
				}


				case MachineInfo::Opcode::JMP_NOT_EQUAL:
				{
					handle_conditional_jump(MachineInfo::FLAGS_MASK_GT | MachineInfo::FLAGS_MASK_LT, pkt);
					break;
				}


				case MachineInfo::Opcode::JMP_GREATER:
				{
					handle_conditional_jump(MachineInfo::FLAGS_MASK_GT, pkt);
					break;
				}

				case MachineInfo::Opcode::JMP_GREATER_EQUAL:
				{
					handle_conditional_jump(MachineInfo::FLAGS_MASK_GT | MachineInfo::FLAGS_MASK_EQ, pkt);
					break;
				}

				case MachineInfo::Opcode::JMP_LOWER:
				{
					handle_conditional_jump(MachineInfo::FLAGS_MASK_LT, pkt);
					break;
				}

				case MachineInfo::Opcode::JMP_LOWER_EQUAL:
				{
					handle_conditional_jump(MachineInfo::FLAGS_MASK_LT | MachineInfo::FLAGS_MASK_EQ, pkt);
					break;
				}

				case MachineInfo::Opcode::LOAD_RESTORE_PC:
				{
					const auto base = static_cast<MachineInfo::RegisterID>((pkt.insn >> 8) & 0xff);
					const auto off1_const = static_cast<int16_t>(pkt.insn >> 16);

					VectorValue off1 = VectorValue::create_vec_int64(off1_const);

					const auto& off2 = regs[base];

					DecodeToExecuteBus::Packet execute_pkt{ PC, MachineInfo::ExecuteStageOpcode::LOAD_RESTORE_PC,
							off1,
							off2,
					};
					execute_bus.send(execute_pkt);
					break;
				}

				// ret = [const + reg]
				case MachineInfo::Opcode::LOAD_REG_CONST_REG:
				{
					const auto dest = static_cast<MachineInfo::RegisterID>((pkt.insn >> 8) & 0xff);
					const auto base = static_cast<MachineInfo::RegisterID>((pkt.insn >> 16) & 0xff);
					const auto off1_const = static_cast<int16_t>(pkt.insn >> 24);

					VectorValue off1 = VectorValue::create_vec_int64(off1_const);

					const auto& off2 = regs[base];

					DecodeToExecuteBus::Packet execute_pkt{ PC, MachineInfo::ExecuteStageOpcode::LOAD_REG,
							dest,
							off1,
							off2,
					};
					execute_bus.send(execute_pkt);
					break;
				}

				case MachineInfo::Opcode::MOVE_R0_CONST24A:
				{
					const auto dest = MachineInfo::RegisterID::R0;
					const auto off1_const = static_cast<int32_t>(pkt.insn >> 8);
					VectorValue off1 = VectorValue::create_vec_int64(off1_const);

					DecodeToExecuteBus::Packet execute_pkt{ PC, MachineInfo::ExecuteStageOpcode::MOVE_REG_VALUE,
							dest,
							off1
					};
					execute_bus.send(execute_pkt);
					break;
				}

				case MachineInfo::Opcode::MOVE_R0_CONST24B:
				{
					const auto dest = MachineInfo::RegisterID::R0;
					const auto off1_const = static_cast<int32_t>(pkt.insn >> 8);
					VectorValue off1 = VectorValue::create_vec_int64(off1_const);
					const auto& off2 = regs[MachineInfo::RegisterID::R0];

					DecodeToExecuteBus::Packet execute_pkt{ PC, MachineInfo::ExecuteStageOpcode::ORB_REG_VALUE,
							dest,
							off1,
							off2
					};
					execute_bus.send(execute_pkt);
					break;
				}

				case MachineInfo::Opcode::MOVE_R0_CONST24C:
				{
					const auto dest = MachineInfo::RegisterID::R0;
					const auto off1_const = static_cast<int32_t>(pkt.insn >> 8);
					VectorValue off1 = VectorValue::create_vec_int64(off1_const);
					const auto& off2 = regs[MachineInfo::RegisterID::R0];

					DecodeToExecuteBus::Packet execute_pkt{ PC, MachineInfo::ExecuteStageOpcode::ORC_REG_VALUE,
							dest,
							off1,
							off2
					};
					execute_bus.send(execute_pkt);
					break;
				}

				default:
					std::cerr << "[DECODE] unimplemented opcode: " << MachineInfo::to_string(opcode) << std::endl;
					abort();
				}
			}


			Task& t = *this;
			co_await t;
		}
	}


	void DecodeStage::handle_conditional_jump(uint32_t jmp_mask_const, const FetchToDecodeBus::Packet& pkt)
	{
		const auto PC = pkt.PC;

		const VectorValue jmp_mask = VectorValue::create_vec_int64(jmp_mask_const);

		const auto off_const = static_cast<int32_t>(pkt.insn >> 8);
		const VectorValue off = VectorValue::create_vec_int64(off_const);

		const auto& flags = regs[MachineInfo::RegisterID::FLAGS];

		const DecodeToExecuteBus::Packet execute_pkt{ PC, MachineInfo::ExecuteStageOpcode::COND_JMP,
			off, jmp_mask, flags };
		execute_bus.send(execute_pkt);
	}
}