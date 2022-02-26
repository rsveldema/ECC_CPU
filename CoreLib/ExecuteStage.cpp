#include "CoreLib.h"

namespace ecc
{

	METHOD_SECTION;

	ReturnObject ExecuteStage::run()
	{
		while (1)
		{
			while (store_bus.is_busy)
			{
				CONTEXT_SWITCH();
			}

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
					// debbug("[EXECUTE] CMP: " + value1 + " -- " + value2 + "-----" + result);

					regs.mark_invalid(RegisterID::REG_FLAGS);

					store_bus.send_reg_vec(pkt.exec_mask, 
							pkt.PC,
							STORAGE_STORE_VALUE_INTO_REG,
							RegisterID::REG_FLAGS,
							compare_vecs(pkt.value0.vec, pkt.value1));
					break;
				}

				case ExecuteStageOpcode::EXEC_MOVE_REG_VALUE:
				{
					regs.mark_invalid(pkt.value0.regID);

					store_bus.send_reg_vec(pkt.exec_mask, 
							pkt.PC,
							STORAGE_STORE_VALUE_INTO_REG,
							pkt.value0.regID,
							pkt.value1);
					break;
				}


				case ExecuteStageOpcode::EXEC_ORB_REG_VALUE:
				{
					regs.mark_invalid(pkt.value0.regID);

					store_bus.send_reg_vec(pkt.exec_mask, 
							pkt.PC,
							STORAGE_STORE_VALUE_INTO_REG,
							pkt.value0.regID,
							or_shift_left(pkt.value2, pkt.value1, 24));

					break;
				}


				case ExecuteStageOpcode::EXEC_ORC_REG_VALUE:
				{
					regs.mark_invalid(pkt.value0.regID);

					store_bus.send_reg_vec(pkt.exec_mask, 
							pkt.PC,
							STORAGE_STORE_VALUE_INTO_REG,
							pkt.value0.regID,
							or_shift_left(pkt.value2, pkt.value1, 48));
					break;
				}

				case ExecuteStageOpcode::EXEC_SHL_REG_VALUE_VALUE:
				{
					regs.mark_invalid(pkt.value0.regID);

					store_bus.send_reg_vec(pkt.exec_mask, 
							pkt.PC,
							STORAGE_STORE_VALUE_INTO_REG,
							pkt.value0.regID,
							shift_left(pkt.value1, pkt.value2));
					break;
				}

				case ExecuteStageOpcode::EXEC_ADD_REG_VALUE_VALUE:
				{
					regs.mark_invalid(pkt.value0.regID);

					store_bus.send_reg_vec(pkt.exec_mask, 
							pkt.PC,
							STORAGE_STORE_VALUE_INTO_REG,
							pkt.value0.regID,
							add(pkt.value1, pkt.value2));
					break;
				}

				// reg = [value + value]
				case ExecuteStageOpcode::EXEC_LOAD_REG:
				{
					regs.mark_invalid(pkt.value0.regID);

					store_bus.send_reg_vec(pkt.exec_mask, 
							pkt.PC,
							STORAGE_LOAD_MEM_INTO_REG,
							pkt.value0.regID,
							add(pkt.value1, pkt.value2));

					break;
				}

				case ExecuteStageOpcode::EXEC_STORE_ADDR_VALUE:
				{
					store_bus.send_vec_vec(pkt.exec_mask, 
							pkt.PC,
							STORAGE_STORE_REG_INTO_MEM,
							add(pkt.value1, pkt.value2),
							pkt.value0.vec);
					break;
				}

				case ExecuteStageOpcode::EXEC_LOAD_RESTORE_PC:
				{
					VectorValue offset = add(pkt.value0.vec, pkt.value1);

					const auto dest = RegisterID::REG_PC;

					ExecStagePacket store_pkt{ pkt.exec_mask, 
						pkt.PC, StorageStageOpcode::STORAGE_LOAD_MEM_INTO_REG,
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
					const auto new_address = pkt.PC + get(pkt.value0.vec, 0);
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
						ExecStagePacket store_pkt{ pkt.exec_mask,
							 pkt.PC, StorageStageOpcode::STORAGE_JMP,
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
					const memory_address_t new_address = pkt.PC + get(pkt.value0.vec, 0);

					ExecStagePacket store_pkt{ pkt.exec_mask, pkt.PC, StorageStageOpcode::STORAGE_JMP, new_address };
					store_bus.send(store_pkt);
					break;
				}

				case ExecuteStageOpcode::EXEC_HALT:
				{
					store_bus.send_none(pkt.exec_mask, 
							pkt.PC,
							STORAGE_HALT);

					break;
				}


				default: {
					$display("[EXECUTE] unhandled opcode in execute stage: ", pkt.opcode);
					abort();
				}
				}
			}


			CONTEXT_SWITCH();
		}
	}
}