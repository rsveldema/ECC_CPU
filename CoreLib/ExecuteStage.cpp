#include "CoreLib.h"

namespace ecc
{

	ReturnObject ExecuteStage::run()
	{
		while (1)
		{
			if (this->decode_bus.is_busy)
			{
				const DecodeExecPacket& pkt = this->decode_bus.recv();
				const auto opcode = pkt.opcode;
				const auto PC = pkt.PC;

				debug("EXECUTE[" + std::to_string(PC) + "] exec: " + to_string(opcode));

				switch (pkt.opcode)
				{
				case ExecuteStageOpcode::EXEC_NOP:
				{
					break;
				}

				case ExecuteStageOpcode::EXEC_CMP:
				{
					const auto& value1 = pkt.value0.vec;
					const auto& value2 = pkt.value1;

					const VectorValue src = value1.compare(value2);

					const auto dest = RegisterID::REG_FLAGS;

					// debbug("[EXECUTE] CMP: " + value1 + " -- " + value2 + "-----" + result);

					while (store_bus.is_busy)
					{
						CONTEXT_SWITCH();
					}

					regs.mark_invalid(dest);

					ExecStagePacket store_pkt{ pkt.exec_mask, pkt.PC,
							StorageStageOpcode::STORAGE_STORE_VALUE_INTO_REG,
							{.regID = dest},
							{.value = src} };
					store_bus.send(store_pkt);
					break;
				}

				case ExecuteStageOpcode::EXEC_MOVE_REG_VALUE:
				{
					const auto& dest = pkt.value0.regID;
					const auto& src = pkt.value1;

					while (store_bus.is_busy)
					{
						CONTEXT_SWITCH();
					}

					regs.mark_invalid(dest);

					ExecStagePacket store_pkt{ pkt.exec_mask, pkt.PC,
						StorageStageOpcode::STORAGE_STORE_VALUE_INTO_REG,
						{.regID = dest}, 
						{.value = src } };
					store_bus.send(store_pkt);
					break;
				}


				case ExecuteStageOpcode::EXEC_ORB_REG_VALUE:
				{
					const auto& dest = pkt.value0.regID;
					const auto& src1 = pkt.value1;
					const auto& src2 = pkt.value2;

					auto src = src2.or_shift_left_int64(src1, 24);

					while (store_bus.is_busy)
					{
						CONTEXT_SWITCH();
					}

					regs.mark_invalid(dest);

					ExecStagePacket store_pkt{ pkt.exec_mask, pkt.PC,
						StorageStageOpcode::STORAGE_STORE_VALUE_INTO_REG,
						{.regID = dest}, 
						{.value = src }};
					store_bus.send(store_pkt);
					break;
				}


				case ExecuteStageOpcode::EXEC_ORC_REG_VALUE:
				{
					const auto& dest = pkt.value0.regID;
					const auto& src1 = pkt.value1;
					const auto& src2 = pkt.value2;

					auto src = src2.or_shift_left_int64(src1, 48);
					while (store_bus.is_busy)
					{
						CONTEXT_SWITCH();
					}

					regs.mark_invalid(dest);

					ExecStagePacket store_pkt{ pkt.exec_mask, pkt.PC,
						StorageStageOpcode::STORAGE_STORE_VALUE_INTO_REG,
						{.regID = dest}, 
						{.value = src }};
					store_bus.send(store_pkt);
					break;
				}

				case ExecuteStageOpcode::EXEC_SHL_REG_VALUE_VALUE:
				{
					const auto& dest = pkt.value0.regID;
					const auto& src1 = pkt.value1;
					const auto& src2 = pkt.value2;

					auto src = src1.shift_left_int64(src2);
					while (store_bus.is_busy)
					{
						CONTEXT_SWITCH();
					}

					regs.mark_invalid(dest);

					ExecStagePacket store_pkt{ pkt.exec_mask, pkt.PC,
						StorageStageOpcode::STORAGE_STORE_VALUE_INTO_REG,
						{.regID = dest }, 
						{.value = src } };
					store_bus.send(store_pkt);
					break;
				}

				case ExecuteStageOpcode::EXEC_ADD_REG_VALUE_VALUE:
				{
					const auto& dest = pkt.value0.regID;
					const auto& src1 = pkt.value1;
					const auto& src2 = pkt.value2;

					auto src = src1.add_int64(src2);

					while (store_bus.is_busy)
					{
						CONTEXT_SWITCH();
					}

					regs.mark_invalid(dest);

					ExecStagePacket store_pkt{ pkt.exec_mask, pkt.PC,
							StorageStageOpcode::STORAGE_STORE_VALUE_INTO_REG,
							{.regID = dest}, 
							{.value = src} 
						};
					store_bus.send(store_pkt);
					break;
				}

				// reg = [value + value]
				case ExecuteStageOpcode::EXEC_LOAD_REG:
				{
					const auto dest = pkt.value0.regID;
					auto off1 = pkt.value1;
					auto off2 = pkt.value2;

					auto offset = off1.add_int64(off2);

					while (store_bus.is_busy)
					{
						CONTEXT_SWITCH();
					}

					regs.mark_invalid(dest);

					ExecStagePacket store_pkt{ pkt.exec_mask, pkt.PC, StorageStageOpcode::STORAGE_LOAD_MEM_INTO_REG,
						{.regID = dest}, 
						{.value = offset} };
					store_bus.send(store_pkt);
					break;
				}

				case ExecuteStageOpcode::EXEC_STORE_ADDR_VALUE:
				{
					const auto& value = pkt.value0.vec;
					const auto& addr1 = pkt.value1;
					const auto& addr2 = pkt.value2;

					auto addr = addr1.add_int64(addr2);

					while (store_bus.is_busy)
					{
						CONTEXT_SWITCH();
					}

					ExecStagePacket store_pkt{ pkt.exec_mask, pkt.PC,
						StorageStageOpcode::STORAGE_STORE_REG_INTO_MEM,
						{.value = addr}, 
						{.value = value} };
					store_bus.send(store_pkt);
					break;
				}

				case ExecuteStageOpcode::EXEC_LOAD_RESTORE_PC:
				{
					const auto& off1 = pkt.value0.vec;
					const auto& off2 = pkt.value1;

					auto offset = off1.add_int64(off2);

					const auto dest = RegisterID::REG_PC;
					while (store_bus.is_busy)
					{
						CONTEXT_SWITCH();
					}

					ExecStagePacket store_pkt{ pkt.exec_mask, pkt.PC, StorageStageOpcode::STORAGE_LOAD_MEM_INTO_REG,
							{.regID =  dest}, 
							{.value = offset}, 
							0 
						};
					store_pkt.is_store_to_pc = true;
					store_bus.send(store_pkt);
					break;
				}


				case ExecuteStageOpcode::EXEC_COND_JMP:
				{
					const auto& offset = pkt.value0.vec;
					const auto PC = pkt.PC;
					const auto new_address = PC + offset.get_PC();
					const auto next_address = PC + 4;

					const auto& jmp_mask = pkt.value1;
					const auto& exec_mask = pkt.exec_mask;

					while (!regs.is_valid(RegisterID::REG_FLAGS))
					{
						CONTEXT_SWITCH();
					}

					const VectorValue flags = regs[RegisterID::REG_FLAGS];
					const auto& should_jmp_masks = flags.bit_and_int64(jmp_mask);
					const uint64_t should_jmp = exec_mask & should_jmp_masks.reduce_int64_to_single_int64_t();
					const uint64_t all_threads_mask = exec_mask & ALL_THREADS_EXEC_MASK_INT64;


					std::cerr << "store-bus busy" << std::endl;
					while (store_bus.is_busy)
					{
						CONTEXT_SWITCH();
					}
					std::cerr << "store-bus done" << std::endl;
					if (should_jmp == 0)
					{
						// no thread wants to jump to the next-insn
						ExecStagePacket store_pkt{ pkt.exec_mask, pkt.PC, StorageStageOpcode::STORAGE_JMP,
							next_address };
						store_bus.send(store_pkt);
					}
					else if (should_jmp == all_threads_mask)
					{
						// all threads just want to go to the next-insn
						ExecStagePacket store_pkt{ pkt.exec_mask, pkt.PC, StorageStageOpcode::STORAGE_JMP,
							new_address };
						store_bus.send(store_pkt);
					}
					else
					{
						// some threads want to jump, some don't.
						execution_mask_t execution_flags_true(should_jmp);
						execution_mask_t execution_flags_false(exec_mask & ~should_jmp);
						bool is_store_to_pc = false;

						ExecStagePacket store_pkt{ pkt.exec_mask, pkt.PC, StorageStageOpcode::STORAGE_CJMP,
							new_address,
							next_address,
							is_store_to_pc,
							execution_flags_true,
							execution_flags_false
						};
						store_bus.send(store_pkt);
					}
					break;
				}

				case ExecuteStageOpcode::EXEC_JMP:
				{
					const auto& offset = pkt.value0.vec;

					const auto PC = pkt.PC;

					const auto new_address = PC + offset.get_PC();

					while (store_bus.is_busy)
					{
						CONTEXT_SWITCH();
					}
					ExecStagePacket store_pkt{ pkt.exec_mask, pkt.PC, StorageStageOpcode::STORAGE_JMP, new_address };
					store_bus.send(store_pkt);
					break;
				}

				case ExecuteStageOpcode::EXEC_HALT:
				{
					while (store_bus.is_busy)
					{
						CONTEXT_SWITCH();
					}
					ExecStagePacket store_pkt{ pkt.exec_mask, 
						pkt.PC, StorageStageOpcode::STORAGE_HALT };
					store_bus.send(store_pkt);
					break;
				}


				default:
					std::cerr << "[EXECUTE] unhandled opcode in execute stage: " << to_string(pkt.opcode) << std::endl;
					abort();
				}
			}


			CONTEXT_SWITCH();
		}
	}
}