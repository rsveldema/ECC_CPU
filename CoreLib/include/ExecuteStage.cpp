#include "CoreLib.h"


namespace Simulator
{

	coro::ReturnObject ExecuteStage::run()
	{
		while (1)
		{
			if (auto pkt_opt = this->decode_bus.try_recv())
			{
				auto pkt = *pkt_opt;
				switch (pkt.opcode)
				{
				case MachineInfo::ExecuteStageOpcode::NOP:
				{
					break;
				}

				case MachineInfo::ExecuteStageOpcode::MOVE_REG_VALUE:
				{
					assert(pkt.dest >= 0);
					assert(pkt.dest < (int)MachineInfo::Register::MAX_REG_ID);
					auto dest = static_cast<MachineInfo::Register>(pkt.dest);
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

					auto dest = MachineInfo::Register::PC;

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

			co_await *this;
		}
	}

}