#include "CoreLib.h"


namespace ecc
{
	template<CoreID core_id>
	ReturnObject StoreStage<core_id>::run()
	{
		while (1)
		{
			if (execute_bus.is_busy)
			{
				const auto& pkt = execute_bus.recv();
				const auto PC = pkt.PC;
				const auto opcode = pkt.opcode;

				switch (opcode)
				{
				case StorageStageOpcode::STORAGE_NOP: break;

				case StorageStageOpcode::STORAGE_STORE_VALUE_INTO_REG:
				{
					const auto dest = pkt.dest.regID;
					assert(isValidIndex(dest));
					const auto& src = pkt.src.value;

					debug("STORE[" + std::to_string(PC) + "] ----> exec: " + to_string(opcode) + " " + to_string(dest) + " = " + to_string(src));

					regs.mark_valid(dest);
					regs[dest] = src;
					break;
				}

				case StorageStageOpcode::STORAGE_STORE_REG_INTO_MEM:
				{
					const auto& dest = pkt.dest.value;
					const auto& src = pkt.src.value;

					assert(are_all_adjacent_memory_addresses(dest, POINTER_SIZE));

					debug("STORE[" + std::to_string(PC) + "] ----> exec: " + to_string(opcode) + " " + to_string(dest) + " = " + to_string(src));

					BusID memory_bus_id = createBusID(core_id, COMPONENT_TYPE_STORE);
					memory_bus.send_write_request_vec(dest, memory_bus_id, src);
					break;
				}

				// reg = memory[src]
				case StorageStageOpcode::STORAGE_LOAD_MEM_INTO_REG:
				{
					auto dest = pkt.dest.regID;
					assert(isValidIndex(dest));
					const auto is_store_to_pc = pkt.is_store_to_pc;

					const auto& src = pkt.src.value;

					debug("STORE[" + std::to_string(PC) + "] ----> exec: " + to_string(opcode) + " " + to_string(dest) + " = " + to_string(src));

					BusID memory_bus_id = createBusID(core_id, COMPONENT_TYPE_STORE);
					memory_bus.send_read_request_vec(src, memory_bus_id);

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
							__global_stats.waitForOperandFetch++;
						}

						CONTEXT_SWITCH();
					}
					break;
				}

				case StorageStageOpcode::STORAGE_JMP:
				{
					auto new_address = pkt.dest.address;

					fetch_bus.send(StoreToFetchPacket{ pkt.exec_mask, new_address });
					break;
				}

				case StorageStageOpcode::STORAGE_CJMP:
				{
					__global_stats.numVectorLocalDivergences++;

					const auto new_address = pkt.dest.address;
					const auto next_address = pkt.src.address;
					const auto& exec_mask_new_address = pkt.execution_flags_true;
					const auto& exec_mask_next_address = pkt.execution_flags_false;

					debug("[STORE] splitting cond-jump into " + std::to_string(count_num_bits64(exec_mask_new_address))
						+ " and " + std::to_string(count_num_bits64(exec_mask_next_address)));

					ThreadContext ctxt{
						regs,
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

						regs = new_thread_ctxt.regs;

						fetch_bus.send(StoreToFetchPacket{ new_thread_ctxt.exec_mask, new_thread_ctxt.PC });
					}
					break;
				}

				default:
					error("unhandled store insn");
					abort();
				}
			}


			CONTEXT_SWITCH();
		}
	}
}