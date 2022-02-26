#include "CoreLib.h"

namespace ecc
{

	METHOD_SECTION;

	execution_mask_t get_cmp_mask(execution_mask_t exec_mask, VectorValue flags, VectorValue value1)
	{
		return (exec_mask & reduce_to_uint64_t(bit_and(flags, value1)));
	}

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
							compare_vecs(pkt.value0.vec, pkt.value1),
							false);
					break;
				}

				case ExecuteStageOpcode::EXEC_MOVE_REG_VALUE:
				{
					regs.mark_invalid(pkt.value0.regID);

					store_bus.send_reg_vec(pkt.exec_mask, 
							pkt.PC,
							STORAGE_STORE_VALUE_INTO_REG,
							pkt.value0.regID,
							pkt.value1,
							false);
					break;
				}


				case ExecuteStageOpcode::EXEC_ORB_REG_VALUE:
				{
					regs.mark_invalid(pkt.value0.regID);

					store_bus.send_reg_vec(pkt.exec_mask, 
							pkt.PC,
							STORAGE_STORE_VALUE_INTO_REG,
							pkt.value0.regID,
							or_shift_left(pkt.value2, pkt.value1, 24),
							false);
					break;
				}


				case ExecuteStageOpcode::EXEC_ORC_REG_VALUE:
				{
					regs.mark_invalid(pkt.value0.regID);

					store_bus.send_reg_vec(pkt.exec_mask, 
							pkt.PC,
							STORAGE_STORE_VALUE_INTO_REG,
							pkt.value0.regID,
							or_shift_left(pkt.value2, pkt.value1, 48),
							false);
					break;
				}

				case ExecuteStageOpcode::EXEC_SHL_REG_VALUE_VALUE:
				{
					regs.mark_invalid(pkt.value0.regID);

					store_bus.send_reg_vec(pkt.exec_mask, 
							pkt.PC,
							STORAGE_STORE_VALUE_INTO_REG,
							pkt.value0.regID,
							shift_left(pkt.value1, pkt.value2),
							false);
					break;
				}

				case ExecuteStageOpcode::EXEC_ADD_REG_VALUE_VALUE:
				{
					regs.mark_invalid(pkt.value0.regID);

					store_bus.send_reg_vec(pkt.exec_mask, 
							pkt.PC,
							STORAGE_STORE_VALUE_INTO_REG,
							pkt.value0.regID,
							add(pkt.value1, pkt.value2),
							false);
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
							add(pkt.value1, pkt.value2),
							false);
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
					store_bus.send_reg_vec(pkt.exec_mask, 
							pkt.PC,
							STORAGE_LOAD_MEM_INTO_REG,
							RegisterID::REG_PC,
							add(pkt.value0.vec, pkt.value1),
							true);
					break;
				}


				case ExecuteStageOpcode::EXEC_COND_JMP:
				{
					while (!regs.is_valid(RegisterID::REG_FLAGS))
					{
						CONTEXT_SWITCH();
					}


					// pkt.value1 = jmp_mask
					//const auto& should_jmp_masks = bit_and(regs.get(RegisterID::REG_FLAGS), pkt.value1);					
					//const uint64_t execution_flags_true = pkt.exec_mask & reduce_to_uint64_t(should_jmp_masks);

					//const uint64_t execution_flags_true = pkt.exec_mask & reduce_to_uint64_t(bit_and(regs.get(RegisterID::REG_FLAGS), pkt.value1));

					

					if (get_cmp_mask(pkt.exec_mask, regs.get(RegisterID::REG_FLAGS), pkt.value1) == 0)
					{
						// no thread wants to jump to the next-insn
						store_bus.send_address(pkt.exec_mask, 
							pkt.PC,
							STORAGE_JMP,
							pkt.PC + sizeof(instruction_t));
					}
					else if (get_cmp_mask(pkt.exec_mask, regs.get(RegisterID::REG_FLAGS), pkt.value1) == (pkt.exec_mask & ALL_THREADS_EXEC_MASK_INT64))
					{
						// all threads just want to go to the next-insn
						store_bus.send_address(pkt.exec_mask, 
							pkt.PC,
							STORAGE_JMP,
							pkt.PC + get(pkt.value0.vec, 0));					
					}
					else
					{
						// some threads want to jump, some don't.
						store_bus.send_2_address(pkt.exec_mask, 
							pkt.PC,
							STORAGE_CJMP,
							pkt.PC + get(pkt.value0.vec, 0),
							pkt.PC + sizeof(instruction_t),							
							get_cmp_mask(pkt.exec_mask, regs.get(RegisterID::REG_FLAGS), pkt.value1),
							(pkt.exec_mask & ~get_cmp_mask(pkt.exec_mask, regs.get(RegisterID::REG_FLAGS), pkt.value1))); // these threads didn't want to jmp 								
					}
					break;
				}

				case ExecuteStageOpcode::EXEC_JMP:
				{
					store_bus.send_address(pkt.exec_mask, 
							pkt.PC,
							STORAGE_JMP,
							pkt.PC + get(pkt.value0.vec, 0));
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