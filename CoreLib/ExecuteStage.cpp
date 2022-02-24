#include "CoreLib.h"

namespace ecc
{

	ReturnObject ExecuteStage::run()
	{
		while (1)
		{
			if (decode_bus.is_busy)
			{
				const DecodeExecPacket pkt = decode_bus.recv();

				CONTEXT_SWITCH();

				$display("EXECUTE: ", pkt.PC, pkt.opcode);

				switch (pkt.opcode)
				{
				case ExecuteStageOpcode::EXEC_NOP:
				{
					break;
				}

				case ExecuteStageOpcode::EXEC_CMP:
				{
					const VectorValue src = compare_vecs(pkt.value0.vec, pkt.value1);

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

					auto src = or_shift_left(pkt.value2, pkt.value1, 24);

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

					auto src = or_shift_left(pkt.value2, pkt.value1, 48);
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

					auto src = shift_left(pkt.value1, pkt.value2);
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

					auto src = add(pkt.value1, pkt.value2);

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
					VectorValue offset = add(pkt.value1, pkt.value2);

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
					auto addr = add(pkt.value1, pkt.value2);

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
					VectorValue offset = add(pkt.value0.vec, pkt.value1);

					const auto dest = RegisterID::REG_PC;
					while (store_bus.is_busy)
					{
						CONTEXT_SWITCH();
					}

					ExecStagePacket store_pkt{ pkt.exec_mask, pkt.PC, StorageStageOpcode::STORAGE_LOAD_MEM_INTO_REG,
							{.regID = dest}, 
							{.value = offset}, 
							0 
						};
					store_pkt.is_store_to_pc = true;
					store_bus.send(store_pkt);
					break;
				}


				case ExecuteStageOpcode::EXEC_COND_JMP:
				{
					const auto new_address = pkt.PC + pkt.value0.vec.get(0);
					const auto next_address = pkt.PC + 4;

					const auto& jmp_mask = pkt.value1;
					const auto& exec_mask = pkt.exec_mask;

					while (!regs.is_valid(RegisterID::REG_FLAGS))
					{
						CONTEXT_SWITCH();
					}

					const VectorValue flags = regs.get(RegisterID::REG_FLAGS);
					const auto& should_jmp_masks = bit_and(flags, jmp_mask);
					const uint64_t should_jmp = exec_mask & reduce_to_uint64_t(should_jmp_masks);
					const uint64_t all_threads_mask = exec_mask & ALL_THREADS_EXEC_MASK_INT64;


					$display("store-bus busy");
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
					const memory_address_t new_address = pkt.PC + pkt.value0.vec.get(0);

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


				default: {
					std::cerr << "[EXECUTE] unhandled opcode in execute stage: " << to_string(pkt.opcode) << std::endl;
					abort();
				}
				}
			}


			CONTEXT_SWITCH();
		}
	}
}