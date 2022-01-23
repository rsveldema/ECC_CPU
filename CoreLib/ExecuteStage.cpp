#include "CoreLib.h"

namespace Simulator
{

	coro::ReturnObject ExecuteStage::run()
	{
		while (1)
		{
			if (const auto pkt_opt = this->decode_bus.try_recv())
			{
				const DecodeToExecuteBus::Packet& pkt = *pkt_opt;
				switch (pkt.opcode)
				{
				case MachineInfo::ExecuteStageOpcode::NOP:
				{
					break;
				}

				case MachineInfo::ExecuteStageOpcode::CMP:
				{
					const auto& value1 = std::get<VectorValue>(pkt.value0);
					const auto& value2 = pkt.value1;

					VectorValue src = value1.compare_int64(value2);

					auto dest = MachineInfo::RegisterID::FLAGS;

					//std::cerr << "[EXECUTE] CMP: " << value1 << " -- " << value2 << "-----" << result << std::endl;

					ExecuteToStoreBus::Packet store_pkt{ pkt.exec_mask, pkt.PC, MachineInfo::StorageStageOpcode::STORE_REG,
						dest,
						src };
					store_bus.send(store_pkt);
					break;
				}

				case MachineInfo::ExecuteStageOpcode::MOVE_REG_VALUE:
				{
					const auto& dest = std::get<MachineInfo::RegisterID>(pkt.value0);
					const auto& src = pkt.value1;

					ExecuteToStoreBus::Packet store_pkt{ pkt.exec_mask, pkt.PC, MachineInfo::StorageStageOpcode::STORE_REG,
						dest, src };

					store_bus.send(store_pkt);
					break;
				}


				case MachineInfo::ExecuteStageOpcode::ORB_REG_VALUE:
				{
					const auto& dest = std::get<MachineInfo::RegisterID>(pkt.value0);
					const auto& src1 = pkt.value1;
					const auto& src2 = pkt.value2;

					auto src = src2.or_shift_left_int64(src1, 24);

					ExecuteToStoreBus::Packet store_pkt{ pkt.exec_mask, pkt.PC, MachineInfo::StorageStageOpcode::STORE_REG,
						dest, src };

					store_bus.send(store_pkt);
					break;
				}


				case MachineInfo::ExecuteStageOpcode::ORC_REG_VALUE:
				{
					const auto& dest = std::get<MachineInfo::RegisterID>(pkt.value0);
					const auto& src1 = pkt.value1;
					const auto& src2 = pkt.value2;

					auto src = src2.or_shift_left_int64(src1, 48);

					ExecuteToStoreBus::Packet store_pkt{ pkt.exec_mask, pkt.PC, MachineInfo::StorageStageOpcode::STORE_REG,
						dest, src };

					store_bus.send(store_pkt);
					break;
				}



				case MachineInfo::ExecuteStageOpcode::SHL_REG_VALUE_VALUE:
				{
					const auto& dest = std::get<MachineInfo::RegisterID>(pkt.value0);
					const auto& src1 = pkt.value1;
					const auto& src2 = pkt.value2;

					auto src = src1.shift_left_int64(src2);

					ExecuteToStoreBus::Packet store_pkt{ pkt.exec_mask, pkt.PC, MachineInfo::StorageStageOpcode::STORE_REG,
						dest, src };

					store_bus.send(store_pkt);
					break;
				}

				case MachineInfo::ExecuteStageOpcode::ADD_REG_VALUE_VALUE:
				{
					const auto& dest = std::get<MachineInfo::RegisterID>(pkt.value0);
					const auto& src1 = pkt.value1;
					const auto& src2 = pkt.value2;

					auto src = src1.add_int64(src2);

					ExecuteToStoreBus::Packet store_pkt{ pkt.exec_mask, pkt.PC, MachineInfo::StorageStageOpcode::STORE_REG,
						dest, src };

					store_bus.send(store_pkt);
					break;
				}

				// reg = [value + value]
				case MachineInfo::ExecuteStageOpcode::LOAD_REG:
				{
					auto dest = std::get<MachineInfo::RegisterID>(pkt.value0);
					auto off1 = pkt.value1;
					auto off2 = pkt.value2;

					auto offset = off1.add_int64(off2);

					ExecuteToStoreBus::Packet store_pkt{ pkt.exec_mask, pkt.PC, MachineInfo::StorageStageOpcode::LOAD_REG,
						dest, offset };

					store_bus.send(store_pkt);
					break;
				}

				case MachineInfo::ExecuteStageOpcode::STORE_ADDR_VALUE:
				{
					const auto& value = std::get<VectorValue>(pkt.value0);
					const auto& addr1 = pkt.value1;
					const auto& addr2 = pkt.value2;

					auto addr = addr1.add_int64(addr2);

					ExecuteToStoreBus::Packet store_pkt{ pkt.exec_mask, pkt.PC, MachineInfo::StorageStageOpcode::STORE_MEM,
						addr, value };

					store_bus.send(store_pkt);
					break;
				}

				case MachineInfo::ExecuteStageOpcode::LOAD_RESTORE_PC:
				{
					const auto& off1 = std::get<VectorValue>(pkt.value0);
					const auto& off2 = pkt.value1;

					auto offset = off1.add_int64(off2);

					auto dest = MachineInfo::RegisterID::PC;

					ExecuteToStoreBus::Packet store_pkt{ pkt.exec_mask, pkt.PC, MachineInfo::StorageStageOpcode::LOAD_REG,
						dest, offset, 0 };
					store_pkt.is_store_to_pc = true;

					store_bus.send(store_pkt);
					break;
				}



				case MachineInfo::ExecuteStageOpcode::COND_JMP:
				{
					const auto& offset = std::get<VectorValue>(pkt.value0);
					const auto PC = pkt.PC;
					const auto new_address = PC + offset.get_PC();
					const auto next_address = PC + 4;

					const auto& jmp_mask = pkt.value1;
					const auto& flags = pkt.value2;
					const auto& exec_mask = pkt.exec_mask;

					const auto& should_jmp_masks = flags.bit_and_int64(jmp_mask);
					const uint64_t should_jmp = exec_mask.get_masked_flags(should_jmp_masks.reduce_int64_to_single_int64_t());
					const uint64_t all_threads_mask = exec_mask.get_masked_flags(MachineInfo::ALL_THREADS_EXEC_MASK_INT64);

					if (should_jmp == 0)
					{
						// no thread wants to jump to the next-insn
						ExecuteToStoreBus::Packet store_pkt{ pkt.exec_mask, pkt.PC, MachineInfo::StorageStageOpcode::JMP,
							next_address };
						store_bus.send(store_pkt);
					}
					else if (should_jmp == all_threads_mask)
					{
						// all threads just want to go to the next-insn
						ExecuteToStoreBus::Packet store_pkt{ pkt.exec_mask, pkt.PC, MachineInfo::StorageStageOpcode::JMP,
							new_address };
						store_bus.send(store_pkt);
					}
					else
					{
						// some threads want to jump, some don't.
						ExecutionMask execution_flags_true(should_jmp);
						ExecutionMask execution_flags_false(exec_mask.get_masked_flags(~should_jmp));
						bool is_store_to_pc = false;

						ExecuteToStoreBus::Packet store_pkt{ pkt.exec_mask, pkt.PC, MachineInfo::StorageStageOpcode::CJMP,
							new_address,
							next_address,
							is_store_to_pc,
							execution_flags_true,
							execution_flags_false
						};
						store_bus.send(store_pkt);
					}

					break;
				}



				case MachineInfo::ExecuteStageOpcode::JMP:
				{
					const auto& offset = std::get<VectorValue>(pkt.value0);

					const auto PC = pkt.PC;

					const auto new_address = PC + offset.get_PC();

					ExecuteToStoreBus::Packet store_pkt{ pkt.exec_mask, pkt.PC, MachineInfo::StorageStageOpcode::JMP, new_address };
					store_bus.send(store_pkt);
					break;
				}

				case MachineInfo::ExecuteStageOpcode::HALT:
				{
					ExecuteToStoreBus::Packet store_pkt{ pkt.exec_mask, pkt.PC, MachineInfo::StorageStageOpcode::HALT };
					store_bus.send(store_pkt);
					break;
				}


				default:
					std::cerr << "[EXECUTE] unhandled opcode in execute stage: " << MachineInfo::to_string(pkt.opcode) << std::endl;
					abort();
				}
			}


			Task& t = *this;
			co_await t;
		}
	}
}