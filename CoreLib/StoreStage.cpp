#include "CoreLib.h"
#include "log_utils.h"


namespace ecc
{

	METHOD_SECTION;

	template<CoreID core_id>
	ReturnObject StoreStage<core_id>::run(ExecuteToStoreBus &execute_bus,
						 VecMemoryBus &memory_bus,
						 RegisterFile &regs,
						 StoreToFetchBus &fetch_bus,
						 DivergenceQueue &divergence_queue,
						 GlobalStats& stats)
	{
		while (1)
		{
			if (execute_bus.is_busy)
			{
				const ExecStagePacket pkt = execute_bus.recv();

				CONTEXT_SWITCH();

				switch (pkt.opcode)
				{
				case STORAGE_NOP: { break; }

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
					assert(are_all_adjacent_memory_addresses(pkt.dest.value, static_cast<int64_t>(POINTER_SIZE)));

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
							stats.incWaitForOperandFetch();
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
					stats.incNumVectorLocalDivergences();

					$display("[STORE] splitting cond-jump: ", 
							count_num_bits64(pkt.execution_flags_true),
							count_num_bits64(pkt.execution_flags_false));
					
					divergence_queue.push_to_front(regs.data, pkt.dest.address, pkt.execution_flags_true);
					divergence_queue.advance_write_pos();

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
						const ThreadContext new_thread_ctxt = divergence_queue.get_back();
						divergence_queue.advance_read_pos();

						CONTEXT_SWITCH();

						regs.data = new_thread_ctxt.regs; 

						fetch_bus.send( new_thread_ctxt.exec_mask, new_thread_ctxt.PC );
					}
					break;
				}

				default: {
					$display("unhandled store insn");
					assert(false);
				}
				}
			}


			CONTEXT_SWITCH();
		}
	}
}