#include "CoreLib.h"


namespace Simulator
{
	coro::ReturnObject StoreStage::run()
	{
		while (1)
		{
			if (auto pkt_opt = this->execute_bus.try_recv())
			{
				auto pkt = *pkt_opt;

				auto PC = pkt.PC;
				auto opcode = pkt.opcode;

				switch (opcode)
				{
				case MachineInfo::StorageStageOpcode::NOP: break;
				case MachineInfo::StorageStageOpcode::STORE_MEM:
				{
					auto addr = pkt.dest;
					auto value = pkt.value;

					unsigned size = 8;

					this->memory_bus.send_write_request(addr, memory_bus_id, size, value);
					break;
				}
				case MachineInfo::StorageStageOpcode::STORE_PC:
				{
					abort();
					break;
				}

				case MachineInfo::StorageStageOpcode::STORE_REG:
				{
					assert(pkt.dest >= 0);
					assert(pkt.dest < (int)MachineInfo::Register::MAX_REG_ID);
					auto src = static_cast<MachineInfo::Register>(pkt.dest);
					auto value = pkt.value;
					regs[src] = value;
					break;
				}

				case MachineInfo::StorageStageOpcode::LOAD_REG:
				{
					auto is_store_to_pc = pkt.is_store_to_pc;
					assert(pkt.dest >= 0);
					assert(pkt.dest < (int)MachineInfo::Register::MAX_REG_ID);
					auto src = static_cast<MachineInfo::Register>(pkt.dest);
					auto addr = pkt.value;

					memory_bus.send_read_request(addr, memory_bus_id, MachineInfo::POINTER_SIZE);

					while (1)
					{
						if (auto new_pc_pkt_opt = memory_bus.try_accept_response())
						{
							auto new_pc_pkt = *new_pc_pkt_opt;
							auto value = *(int64_t*)&new_pc_pkt.payload[0];

							regs[src] = value;

							//std::cerr << "REG[" << MachineInfo::to_string(src) << "] = " << std::to_string(value) << std::endl;

							if (is_store_to_pc)
							{
								fetch_bus.send(StoreToFetchBus::Packet{ value });
							}
							break;
						}
						else
						{
							stats.waitForOperandFetch++;
						}

						co_await *this;
					}


					break;
				}


				case MachineInfo::StorageStageOpcode::JMP:
				{
					auto new_address = pkt.dest;

					fetch_bus.send(StoreToFetchBus::Packet{ new_address });
					break;
				}

				default:
					std::cerr << "unhandled store insn" << std::endl;
					abort();
				}
			}

			co_await *this;
		}
	}

}