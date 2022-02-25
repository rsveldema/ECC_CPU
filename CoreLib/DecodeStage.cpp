#include <cassert>

#include "CoreLib.h"
#include "MachineInfo.h"
#include "log_utils.h"

namespace ecc
{

	METHOD_SECTION;

	flags_reg_t get_expected_cond_jump_flags_mask(Opcode opcode)
	{
		switch (opcode)
		{
		case INSN_OPCODE_JMP_EQUAL:
		{
			return FLAGS_MASK_EQ;
		}
		case INSN_OPCODE_JMP_NOT_EQUAL:
		{
			return FLAGS_MASK_LT | FLAGS_MASK_GT;
		}

		case INSN_OPCODE_JMP_GREATER:
		{
			return FLAGS_MASK_GT;
		}
		case INSN_OPCODE_JMP_LOWER:
		{
			return FLAGS_MASK_LT;
		}

		case INSN_OPCODE_JMP_GREATER_EQUAL:
		{
			return FLAGS_MASK_GT | FLAGS_MASK_EQ;
		}
		case INSN_OPCODE_JMP_LOWER_EQUAL:
		{
			return FLAGS_MASK_LT | FLAGS_MASK_EQ;
		}
		default:
		{
			assert(false);
			return FLAGS_MASK_GT;
		}
		}
		assert(false);
		return FLAGS_MASK_GT;
	}

	template <CoreID core_id>
	ReturnObject DecodeStage<core_id>::run(FetchToDecodeBus &fetch_bus,
										   DecodeToExecuteBus &execute_bus,
										   RegisterFile &regs)
	{
		DecodeStageValue value0;
		VectorValue value1;
		VectorValue value2;

		while (1)
		{
			while (execute_bus.is_busy)
			{
				CONTEXT_SWITCH();
			}

			if (fetch_bus.is_busy)
			{
				const FetchToDecodeBusPacket pkt = fetch_bus.recv();
				CONTEXT_SWITCH();

				$display("DECODE exec: ", pkt.PC, static_cast<Opcode>(pkt.insn));

				switch (static_cast<Opcode>(pkt.insn))
				{
				case INSN_OPCODE_NOP:
				{
					break;
				}

				case INSN_OPCODE_HALT:
				{
					execute_bus.send_req1(pkt.exec_mask, pkt.PC, EXEC_HALT, value0);
					break;
				}

				case INSN_OPCODE_MOVE_PCREL_REG_CONST16:
				{
					value0.regID = static_cast<RegisterID>((pkt.insn >> 8));
					value1 = create_vec_int64(static_cast<int64_t>(static_cast<uint16_t>(pkt.insn >> 16)) 
												+ pkt.PC);

					execute_bus.send_req2(pkt.exec_mask,
										  pkt.PC,
										  EXEC_MOVE_REG_VALUE,
										  value0,
										  value1);
					break;
				}

				// reg = [reg + const]
				case INSN_OPCODE_STORE_REG_CONST_REG:
				{
					while ((!regs.is_valid(static_cast<RegisterID>((pkt.insn >> 8)))) |
						   (!regs.is_valid(static_cast<RegisterID>((pkt.insn >> 16)))))
					{
						CONTEXT_SWITCH();
					}

					value0.vec = regs.get(static_cast<RegisterID>((pkt.insn >> 8)));
					value2 = regs.get(static_cast<RegisterID>((pkt.insn >> 16)));
					value1 = create_vec_int16(static_cast<int16_t>(static_cast<uint8_t>(pkt.insn >> 24)));

					execute_bus.send_req3(pkt.exec_mask, pkt.PC, EXEC_STORE_ADDR_VALUE,
										  value0, value1, value2);
					break;
				}

				// reg = blockIndex
				case INSN_OPCODE_MOVE_REG_BLOCK_INDEX:
				{
					value0.regID = static_cast<RegisterID>((pkt.insn >> 8));
					value1 = create_vec_incrementing_values();

					execute_bus.send_req2(pkt.exec_mask, pkt.PC, EXEC_MOVE_REG_VALUE,
										  value0, value1);
					break;
				}

				case INSN_OPCODE_MOVE_REG_CONST16:
				{
					value0.regID = static_cast<RegisterID>((pkt.insn >> 8));
					value1 = create_vec_int16(static_cast<int16_t>((pkt.insn >> 16)));

					execute_bus.send_req2(pkt.exec_mask, pkt.PC, EXEC_MOVE_REG_VALUE,
										  value0, value1);
					break;
				}

				case INSN_OPCODE_MOVE_REG_REG:
				{
					while (!regs.is_valid(static_cast<RegisterID>((pkt.insn >> 16))))
					{
						CONTEXT_SWITCH();
					}

					value0.regID = static_cast<RegisterID>((pkt.insn >> 8));
					value1 = regs.get(static_cast<RegisterID>((pkt.insn >> 16)));

					execute_bus.send_req2(pkt.exec_mask, pkt.PC, EXEC_MOVE_REG_VALUE,
										  value0, value1);
					break;
				}

				case INSN_OPCODE_L_SSHIFT_REG_REG_CONST:
				{
					while (!regs.is_valid(static_cast<RegisterID>((pkt.insn >> 16))))
					{
						CONTEXT_SWITCH();
					}

					value0.regID = static_cast<RegisterID>((pkt.insn >> 8));
					value1 = regs.get(static_cast<RegisterID>((pkt.insn >> 16)));
					value2 = create_vec_int8(static_cast<int8_t>((pkt.insn >> 24)));

					execute_bus.send_req3(pkt.exec_mask, pkt.PC, EXEC_SHL_REG_VALUE_VALUE,
										  value0, value1, value2);
					break;
				}

				case INSN_OPCODE_ADD_REG_REG_REG:
				{
					while ((!regs.is_valid(static_cast<RegisterID>((pkt.insn >> 16)))) |
							(!regs.is_valid(static_cast<RegisterID>((pkt.insn >> 24)))))
					{
						CONTEXT_SWITCH();
					}


					value0.regID = static_cast<RegisterID>((pkt.insn >> 8) );
					value1 = regs.get(static_cast<RegisterID>((pkt.insn >> 16)));
					value2 = regs.get(static_cast<RegisterID>((pkt.insn >> 24)));

					execute_bus.send_req3(pkt.exec_mask, pkt.PC,
										  EXEC_ADD_REG_VALUE_VALUE,
										  value0, value1, value2);
					break;
				}

				// reg = reg + const
				case INSN_OPCODE_ADD_REG_REG_CONST:
				{
					while (!regs.is_valid(static_cast<RegisterID>((pkt.insn >> 16))))
					{
						CONTEXT_SWITCH();
					}

					value0.regID = static_cast<RegisterID>((pkt.insn >> 8));
					value2 = create_vec_int8(static_cast<int8_t>((pkt.insn >> 24)));
					value1 = regs.get(static_cast<RegisterID>((pkt.insn >> 16)));

					execute_bus.send_req3(pkt.exec_mask, pkt.PC, EXEC_ADD_REG_VALUE_VALUE,
										  value0,
										  value1,
										  value2);
					break;
				}

				case INSN_OPCODE_JMP_ALWAYS:
				{
					value0.vec = create_vec_int32(static_cast<int32_t>(pkt.insn >> 8));

					execute_bus.send_req1(pkt.exec_mask, pkt.PC, EXEC_JMP,
										  value0);
					break;
				}

				case INSN_OPCODE_CMP_REG_REG:
				{					
					while ((!regs.is_valid(static_cast<RegisterID>((pkt.insn >> 8)))) |
							(!regs.is_valid(static_cast<RegisterID>((pkt.insn >> 16)))))
					{
						CONTEXT_SWITCH();
					}

					value0.vec = regs.get(static_cast<RegisterID>((pkt.insn >> 8)));
					value1 = regs.get(static_cast<RegisterID>((pkt.insn >> 16)));

					// std::cerr << "[DECODE] CMP: " << value1 << " -- " << value2 << std::endl;

					execute_bus.send_req2(pkt.exec_mask, pkt.PC, EXEC_CMP,
										  value0,
										  value1);
					break;
				}

				case INSN_OPCODE_JMP_LOWER:
				case INSN_OPCODE_JMP_LOWER_EQUAL:
				case INSN_OPCODE_JMP_EQUAL:
				case INSN_OPCODE_JMP_NOT_EQUAL:
				case INSN_OPCODE_JMP_GREATER:
				case INSN_OPCODE_JMP_GREATER_EQUAL:
				{
					
					value1 = create_vec_int64(get_expected_cond_jump_flags_mask(static_cast<Opcode>(pkt.insn)));
					value0.vec = create_vec_int32(static_cast<int32_t>(pkt.insn >> 8));

					execute_bus.send_req2(pkt.exec_mask, pkt.PC,
										  EXEC_COND_JMP,
										  value0, value1);
					break;
				}

				case INSN_OPCODE_LOAD_RESTORE_PC:
				{
					while (!regs.is_valid(static_cast<RegisterID>((pkt.insn >> 8))))
					{
						CONTEXT_SWITCH();
					}

					value0.vec = create_vec_int16(static_cast<int16_t>(pkt.insn >> 16));
					value1 = regs.get(static_cast<RegisterID>((pkt.insn >> 8)));

					execute_bus.send_req2(pkt.exec_mask, pkt.PC, EXEC_LOAD_RESTORE_PC,
										  value0,
										  value1);
					break;
				}

				// ret = [const + reg]
				case INSN_OPCODE_LOAD_REG_CONST_REG:
				{
					while (!regs.is_valid(static_cast<RegisterID>((pkt.insn >> 16))))
					{
						CONTEXT_SWITCH();
					}

					value0.regID = static_cast<RegisterID>((pkt.insn >> 8));
					value2 = regs.get(static_cast<RegisterID>((pkt.insn >> 16)));
					value1 = create_vec_int16(static_cast<int16_t>(pkt.insn >> 24));

					execute_bus.send_req3(pkt.exec_mask, pkt.PC, EXEC_LOAD_REG,
										  value0,
										  value1,
										  value2);
					break;
				}

				case INSN_OPCODE_MOVE_R0_CONST24A:
				{
					value0.regID = REG_R0;
					value1 = create_vec_int32(static_cast<int32_t>(pkt.insn >> 8));

					execute_bus.send_req2(pkt.exec_mask, pkt.PC, EXEC_MOVE_REG_VALUE,
										  value0,
										  value1);
					break;
				}

				case INSN_OPCODE_MOVE_R0_CONST24B:
				{
					while (!regs.is_valid(REG_R0))
					{
						CONTEXT_SWITCH();
					}
					value0.regID = REG_R0;
					value1 = create_vec_int32(static_cast<int32_t>(pkt.insn >> 8));
					value2 = regs.get(REG_R0);

					execute_bus.send_req3(pkt.exec_mask, pkt.PC, EXEC_ORB_REG_VALUE,
										  value0,
										  value1,
										  value2);
					break;
				}

				case INSN_OPCODE_MOVE_R0_CONST24C:
				{
					while (!regs.is_valid(REG_R0))
					{
						CONTEXT_SWITCH();
					}
					value0.regID = REG_R0;
					value1 = create_vec_int32(static_cast<int32_t>(pkt.insn >> 8));
					value2 = regs.get(REG_R0);

					execute_bus.send_req3(pkt.exec_mask, pkt.PC,
										  EXEC_ORC_REG_VALUE,
										  value0,
										  value1,
										  value2);
					break;
				}

				default:
				{
					$display("[DECODE] unimplemented opcode: ", static_cast<Opcode>(pkt.insn));
					assert(false);
				}
				}
			}

			CONTEXT_SWITCH();
		}
	}

}