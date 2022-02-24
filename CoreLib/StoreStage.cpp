#include "CoreLib.h"
#include "log_utils.h"


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

				CONTEXT_SWITCH();

				switch (pkt.opcode)
				{
				case STORAGE_NOP: break;

				case STORAGE_STORE_VALUE_INTO_REG:
				{
					const auto dest = pkt.dest.regID;
					assert(isValidIndex(dest));
					const auto& src = pkt.src.value;

					$display("STORE ----> exec: ", dest, src);

					regs.mark_valid(dest);
					regs.set(dest, src);
					break;
				}

				case STORAGE_STORE_REG_INTO_MEM:
				{
					const auto& dest = pkt.dest.value;
					const auto& src = pkt.src.value;

					assert(are_all_adjacent_memory_addresses(dest, POINTER_SIZE));

					$display("STORE ----> exec: ", dest, src);

					BusID memory_bus_id = createBusID(core_id, COMPONENT_TYPE_STORE);
					memory_bus.send_write_request_vec(dest, memory_bus_id, src);
					break;
				}

				// reg = memory[src]
				case STORAGE_LOAD_MEM_INTO_REG:
				{
					auto dest = pkt.dest.regID;
					assert(isValidIndex(dest));
					const auto is_store_to_pc = pkt.is_store_to_pc;

					const auto& src = pkt.src.value;

					$display("STORE ----> exec: ", dest, src);

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
							regs.set(dest, value);

							//$display("REG[" << MachineInfo::to_string(src) << "] = " << std::to_string(value));
							if (is_store_to_pc)
							{
								memory_address_t new_pc = get(value, 0);
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

				case STORAGE_JMP:
				{
					auto new_address = pkt.dest.address;

					fetch_bus.send(StoreToFetchPacket{ pkt.exec_mask, new_address });
					break;
				}

				case STORAGE_CJMP:
				{
					__global_stats.numVectorLocalDivergences += 1;

					const auto new_address = pkt.dest.address;
					const auto next_address = pkt.src.address;
					const auto& exec_mask_new_address = pkt.execution_flags_true;
					const auto& exec_mask_next_address = pkt.execution_flags_false;

					$display("[STORE] splitting cond-jump: ", 
							count_num_bits64(exec_mask_new_address),
							count_num_bits64(exec_mask_next_address));

					ThreadContext ctxt{
						regs,
						new_address,
						exec_mask_new_address
					};
					divergence_queue.push_front(ctxt);

					fetch_bus.send(StoreToFetchPacket{ exec_mask_next_address, next_address });
					break;
				}

				case STORAGE_HALT:
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
					$display("unhandled store insn");
					assert(false);
				}
			}


			CONTEXT_SWITCH();
		}
	}
}