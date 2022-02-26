#include "CoreLib.h"
#include "log_utils.h"


namespace ecc
{

	METHOD_SECTION;

	template<CoreID core_id>
	ReturnObject StoreStage<core_id>::run()
	{
		while (1)
		{
			if (execute_bus.is_busy)
			{
				const ExecStagePacket pkt = execute_bus.recv();

				CONTEXT_SWITCH();

				switch (pkt.opcode)
				{
				case STORAGE_NOP: break;

				case STORAGE_STORE_VALUE_INTO_REG:
				{
					assert(isValidIndex(pkt.dest.regID));

					$display("STORE ----> exec: ", pkt.dest.regID,  pkt.src.value);

					regs.mark_valid(pkt.dest.regID);
					regs.set(pkt.dest.regID,  pkt.src.value);
					break;
				}

				case STORAGE_STORE_REG_INTO_MEM:
				{
					assert(are_all_adjacent_memory_addresses(pkt.dest.value, POINTER_SIZE));

					$display("STORE ----> exec: ", pkt.dest.value, pkt.src.value);

					memory_bus.send_write_request_vec(pkt.dest.value, 
								createBusID(core_id, COMPONENT_TYPE_STORE), 
								pkt.src.value);
					break;
				}

				// reg = memory[src]
				case STORAGE_LOAD_MEM_INTO_REG:
				{
					assert(isValidIndex(pkt.dest.regID));

					$display("STORE ----> exec: ", pkt.dest.regID, pkt.src.value);

					memory_bus.send_read_request_vec(pkt.src.value, createBusID(core_id, COMPONENT_TYPE_STORE));

					while (1)
					{
						if (memory_bus.response_busy)
						{
							const VecBusPacket new_pc_pkt = memory_bus.accept_response();
							CONTEXT_SWITCH();

							regs.mark_valid(pkt.dest.regID);
							regs.set(pkt.dest.regID, new_pc_pkt.payload);

							//$display("REG[" << MachineInfo::to_string(src) << "] = " << std::to_string(value));
							if (pkt.is_store_to_pc)
							{
								fetch_bus.send(pkt.exec_mask, get(new_pc_pkt.payload, 0));
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
					fetch_bus.send(pkt.exec_mask, pkt.dest.address);
					break;
				}

				case STORAGE_CJMP:
				{
					__global_stats.numVectorLocalDivergences += 1;

					//const auto new_address = pkt.dest.address;
					//const auto next_address = pkt.src.address;
					//const auto& exec_mask_new_address = pkt.execution_flags_true;
					//const auto& exec_mask_next_address = pkt.execution_flags_false;

					$display("[STORE] splitting cond-jump: ", 
							count_num_bits64(pkt.execution_flags_true),
							count_num_bits64(pkt.execution_flags_false));

					ThreadContext ctxt{
						regs,
						pkt.dest.address,
						pkt.execution_flags_true
					};
					divergence_queue.push_front(ctxt);

					fetch_bus.send(pkt.execution_flags_false, pkt.src.address);
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

						fetch_bus.send( new_thread_ctxt.exec_mask, new_thread_ctxt.PC );
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