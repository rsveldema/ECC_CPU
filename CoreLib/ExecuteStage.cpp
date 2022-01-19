#include "CoreLib.h"


namespace Simulator
{

	coro::ReturnObject ExecuteStage::run()
	{
		while (1)
		{
			if (auto pkt_opt = this->decode_bus.try_recv())
			{
				DecodeToExecuteBus::Packet& pkt = *pkt_opt;
				switch (pkt.opcode)
				{
				case MachineInfo::ExecuteStageOpcode::NOP:
				{
					break;
				}

				case MachineInfo::ExecuteStageOpcode::CMP:
				{
					int64_t value1 = pkt.dest;
					int64_t value2 = pkt.value1;

					auto result = 0;

					if (value1 == value2)
						result |= MachineInfo::FLAGS_MASK_EQ;

					if (value1 > value2)
						result |= MachineInfo::FLAGS_MASK_GT;

					if (value1 < value2)
						result |= MachineInfo::FLAGS_MASK_LT;

					auto dest = MachineInfo::RegisterID::FLAGS;
					auto src = result;

					//std::cerr << "[EXECUTE] CMP: " << value1 << " -- " << value2 << "-----" << result << std::endl;

					ExecuteToStoreBus::Packet store_pkt{ pkt.PC, MachineInfo::StorageStageOpcode::STORE_REG, (int64_t)dest, src };
					store_bus.send(store_pkt);
					break;
				}

				case MachineInfo::ExecuteStageOpcode::MOVE_REG_VALUE:
				{
					assert(pkt.dest >= 0);
					assert(pkt.dest < (int)MachineInfo::RegisterID::MAX_REG_ID);
					auto dest = static_cast<MachineInfo::RegisterID>(pkt.dest);
					auto src = pkt.value1;

					ExecuteToStoreBus::Packet store_pkt{ pkt.PC, MachineInfo::StorageStageOpcode::STORE_REG, (int64_t)dest, src };

					store_bus.send(store_pkt);
					break;
				}

				case MachineInfo::ExecuteStageOpcode::STORE_ADDR_VALUE:
				{
					int64_t value = pkt.dest;
					auto addr1 = pkt.value1;
					auto addr2 = pkt.value2;

					auto addr = addr1 + addr2;

					ExecuteToStoreBus::Packet store_pkt{ pkt.PC, MachineInfo::StorageStageOpcode::STORE_MEM, addr, value };

					store_bus.send(store_pkt);
					break;
				}

				case MachineInfo::ExecuteStageOpcode::LOAD_RESTORE_PC:
				{
					int64_t off1 = pkt.dest;
					int64_t off2 = pkt.value1;

					auto offset = off1 + off2;

					auto dest = MachineInfo::RegisterID::PC;

					ExecuteToStoreBus::Packet store_pkt{ pkt.PC, MachineInfo::StorageStageOpcode::LOAD_REG,
						(int64_t)dest, offset, 0 };
					store_pkt.is_store_to_pc = true;

					store_bus.send(store_pkt);
					break;
				}

				case MachineInfo::ExecuteStageOpcode::LOAD_REG:
				{
					auto dest = pkt.dest;
					int64_t off1 = pkt.value1;
					int64_t off2 = pkt.value2;

					auto offset = off1 + off2;

					ExecuteToStoreBus::Packet store_pkt{ pkt.PC, MachineInfo::StorageStageOpcode::LOAD_REG,
						dest, offset };

					store_bus.send(store_pkt);
					break;
				}


				case MachineInfo::ExecuteStageOpcode::COND_JMP:
				{
					int64_t offset = pkt.dest;
					auto PC = pkt.PC;
					auto new_address = PC + offset;
					auto next_address = PC + 4;

					auto jmp_mask = pkt.value1;
					auto flags = pkt.value2;

					bool should_jmp = flags & jmp_mask;

					if (should_jmp)
					{
						ExecuteToStoreBus::Packet store_pkt{ pkt.PC, MachineInfo::StorageStageOpcode::JMP, (int64_t)new_address, should_jmp };
						store_bus.send(store_pkt);
					}
					else
					{
						ExecuteToStoreBus::Packet store_pkt{ pkt.PC, MachineInfo::StorageStageOpcode::JMP, (int64_t)next_address, should_jmp };
						store_bus.send(store_pkt);
					}
					break;
				}



				case MachineInfo::ExecuteStageOpcode::JMP:
				{
					int64_t offset = pkt.dest;

					auto PC = pkt.PC;

					auto new_address = PC + offset;

					ExecuteToStoreBus::Packet store_pkt{ pkt.PC, MachineInfo::StorageStageOpcode::JMP, (int64_t)new_address };
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