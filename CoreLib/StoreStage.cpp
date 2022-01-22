#include "CoreLib.h"


namespace Simulator
{
	coro::ReturnObject StoreStage::run()
	{
		while (1)
		{
			if (const auto pkt_opt = this->execute_bus.try_recv())
			{
				const auto& pkt = *pkt_opt;

				auto PC = pkt.PC;
				auto opcode = pkt.opcode;

				switch (opcode)
				{
				case MachineInfo::StorageStageOpcode::NOP: break;

				case MachineInfo::StorageStageOpcode::STORE_REG:
				{
					const auto dest = std::get<MachineInfo::RegisterID>(pkt.dest);
					assert(isValid(dest));
					const auto& src = std::get<VectorValue>(pkt.src);

					regs[dest] = src;
					break;
				}

				case MachineInfo::StorageStageOpcode::STORE_MEM:
				{
					const auto& dest = std::get<VectorValue>(pkt.dest);
					const auto& src = std::get<VectorValue>(pkt.src);

					assert(dest.are_all_adjacent_memory_addresses(8));

					this->memory_bus.send_write_request_vec(dest.get_int64_array(), memory_bus_id, src);
					break;
				}

				// reg = memory[src]
				case MachineInfo::StorageStageOpcode::LOAD_REG:
				{
					auto dest = std::get<MachineInfo::RegisterID>(pkt.dest);
					assert(isValid(dest));
					const auto is_store_to_pc = pkt.is_store_to_pc;

					const auto& src = std::get<VectorValue>(pkt.src);

					memory_bus.send_read_request_vec(src.get_int64_array(), memory_bus_id);

					while (1)
					{
						if (auto new_pc_pkt_opt = memory_bus.try_accept_response())
						{
							const auto& new_pc_pkt = *new_pc_pkt_opt;
							const auto& payload = new_pc_pkt.payload;
							const auto& value = std::get<VectorValue>(payload);

							regs[dest] = value;

							//std::cerr << "REG[" << MachineInfo::to_string(src) << "] = " << std::to_string(value) << std::endl;


							if (is_store_to_pc)
							{
								auto new_pc = value.get_PC();
								fetch_bus.send(StoreToFetchBus::Packet{ new_pc });
							}
							break;
						}
						else
						{
							stats.waitForOperandFetch++;
						}

						Task& t = *this;
						co_await t;
					}
					break;
				}

				case MachineInfo::StorageStageOpcode::JMP:
				{
					auto new_address = std::get<MachineInfo::memory_address_t>(pkt.dest);

					fetch_bus.send(StoreToFetchBus::Packet{ new_address });
					break;
				}

				default:
					std::cerr << "unhandled store insn" << std::endl;
					abort();
				}
			}


			Task& t = *this;
			co_await t;
		}
	}

}