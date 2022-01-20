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

					ExecuteToStoreBus::Packet store_pkt{ pkt.PC, MachineInfo::StorageStageOpcode::STORE_REG,
						dest,
						src };
					store_bus.send(store_pkt);
					break;
				}

				case MachineInfo::ExecuteStageOpcode::MOVE_REG_VALUE:
				{
					const auto& dest = std::get<MachineInfo::RegisterID>(pkt.value0);
					const auto& src = pkt.value1;

					ExecuteToStoreBus::Packet store_pkt{ pkt.PC, MachineInfo::StorageStageOpcode::STORE_REG,
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

					ExecuteToStoreBus::Packet store_pkt{ pkt.PC, MachineInfo::StorageStageOpcode::STORE_REG,
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

					ExecuteToStoreBus::Packet store_pkt{ pkt.PC, MachineInfo::StorageStageOpcode::STORE_REG,
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

					ExecuteToStoreBus::Packet store_pkt{ pkt.PC, MachineInfo::StorageStageOpcode::LOAD_REG,
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

					ExecuteToStoreBus::Packet store_pkt{ pkt.PC, MachineInfo::StorageStageOpcode::STORE_MEM,
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

					ExecuteToStoreBus::Packet store_pkt{ pkt.PC, MachineInfo::StorageStageOpcode::LOAD_REG,
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

					const auto should_jmp = flags.bit_and_int64(jmp_mask);

					if (should_jmp.all_equal_int64())
					{
						if (should_jmp.get_int64(0))
						{
							ExecuteToStoreBus::Packet store_pkt{ pkt.PC, MachineInfo::StorageStageOpcode::JMP,
								new_address, should_jmp };
							store_bus.send(store_pkt);
						}
						else
						{
							ExecuteToStoreBus::Packet store_pkt{ pkt.PC, MachineInfo::StorageStageOpcode::JMP,
								next_address, should_jmp };
							store_bus.send(store_pkt);
						}
					}
					else
					{
						// divergence: we should let some jmp and some not.
						abort();
					}
					break;
				}



				case MachineInfo::ExecuteStageOpcode::JMP:
				{
					const auto& offset = std::get<VectorValue>(pkt.value0);

					const auto PC = pkt.PC;

					const auto new_address = PC + offset.get_PC();

					ExecuteToStoreBus::Packet store_pkt{ pkt.PC, MachineInfo::StorageStageOpcode::JMP, new_address };
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