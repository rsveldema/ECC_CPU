#include "CoreLib.h"


namespace ecc
{
	ReturnObject StoreStage::run()
	{
		while (1)
		{
			if (const auto pkt_opt = this->execute_bus.try_recv())
			{
				const auto& pkt = *pkt_opt;
				const auto PC = pkt.PC;
				const auto opcode = pkt.opcode;

				switch (opcode)
				{
				case StorageStageOpcode::STORAGE_NOP: break;

				case StorageStageOpcode::STORAGE_STORE_VALUE_INTO_REG:
				{
					const auto dest = std::get<RegisterID>(pkt.dest);
					assert(isValidIndex(dest));
					const auto& src = std::get<VectorValue>(pkt.src);

					logger.debug("STORE[" + std::to_string(PC) + "] ----> exec: " + to_string(opcode) + " " + to_string(dest) + " = " + to_string(src));

					regs.mark_valid(dest);
					regs[dest] = src;
					break;
				}

				case StorageStageOpcode::STORAGE_STORE_REG_INTO_MEM:
				{
					const auto& dest = std::get<VectorValue>(pkt.dest);
					const auto& src = std::get<VectorValue>(pkt.src);

					assert(dest.are_all_adjacent_memory_addresses(8));

					logger.debug("STORE[" + std::to_string(PC) + "] ----> exec: " + to_string(opcode) + " " + to_string(dest) + " = " + to_string(src));

					this->memory_bus.send_write_request_vec(dest.get_int64_array(), memory_bus_id, src);
					break;
				}

				// reg = memory[src]
				case StorageStageOpcode::STORAGE_LOAD_MEM_INTO_REG:
				{
					auto dest = std::get<RegisterID>(pkt.dest);
					assert(isValidIndex(dest));
					const auto is_store_to_pc = pkt.is_store_to_pc;

					const auto& src = std::get<VectorValue>(pkt.src);

					logger.debug("STORE[" + std::to_string(PC) + "] ----> exec: " + to_string(opcode) + " " + to_string(dest) + " = " + to_string(src));

					memory_bus.send_read_request_vec(src.get_int64_array(), memory_bus_id);

					while (1)
					{
						if (auto new_pc_pkt_opt = memory_bus.try_accept_response())
						{
							const auto& new_pc_pkt = *new_pc_pkt_opt;
							const auto& payload = new_pc_pkt.payload;
							const auto& value = payload;

							regs.mark_valid(dest);
							regs[dest] = value;

							//std::cerr << "REG[" << MachineInfo::to_string(src) << "] = " << std::to_string(value) << std::endl;
							if (is_store_to_pc)
							{
								auto new_pc = value.get_PC();
								fetch_bus.send(StoreToFetchPacket{ pkt.exec_mask, new_pc });
							}
							break;
						}
						else
						{
							stats.waitForOperandFetch++;
						}

						CONTEXT_SWITCH();
					}
					break;
				}

				case StorageStageOpcode::STORAGE_JMP:
				{
					auto new_address = std::get<memory_address_t>(pkt.dest);

					fetch_bus.send(StoreToFetchPacket{ pkt.exec_mask, new_address });
					break;
				}

				case StorageStageOpcode::STORAGE_CJMP:
				{
					stats.numVectorLocalDivergences++;

					const auto new_address = std::get<memory_address_t>(pkt.dest);
					const auto next_address = std::get<memory_address_t>(pkt.src);
					const auto& exec_mask_new_address = pkt.execution_flags_true;
					const auto& exec_mask_next_address = pkt.execution_flags_false;

					logger.debug("[STORE] splitting cond-jump into " + std::to_string(exec_mask_new_address.count())
						+ " and " + std::to_string(exec_mask_next_address.count()));

					ThreadContext ctxt{
						this->regs,
						new_address,
						exec_mask_new_address
					};
					divergence_queue.push_front(ctxt);

					fetch_bus.send(StoreToFetchPacket{ exec_mask_next_address, next_address });
					break;
				}

				case StorageStageOpcode::STORAGE_HALT:
				{
					if (divergence_queue.is_empty())
					{
						regs.setHasHalted();
					}
					else
					{
						const auto& new_thread_ctxt_opt = divergence_queue.pop_back();
						const auto& new_thread_ctxt = *new_thread_ctxt_opt;

						this->regs = new_thread_ctxt.regs;

						fetch_bus.send(StoreToFetchPacket{ new_thread_ctxt.exec_mask, new_thread_ctxt.PC });
					}
					break;
				}

				default:
					std::cerr << "unhandled store insn" << std::endl;
					abort();
				}
			}


			CONTEXT_SWITCH();
		}
	}
}