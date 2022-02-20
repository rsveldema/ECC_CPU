#include "CoreLib.h"
#include "MachineInfo.h"
#include <cassert>

namespace ecc
{

METHOD_SECTION;

	uint64_t get_expected_cond_jump_flags_mask(Opcode opcode)
	{
		switch (opcode)
		{
		case Opcode::INSN_OPCODE_JMP_EQUAL:		{return FLAGS_MASK_EQ;}
		case Opcode::INSN_OPCODE_JMP_NOT_EQUAL:	{return FLAGS_MASK_LT | FLAGS_MASK_GT;}

		case Opcode::INSN_OPCODE_JMP_GREATER:	{return FLAGS_MASK_GT;}
		case Opcode::INSN_OPCODE_JMP_LOWER:		{return FLAGS_MASK_LT;}

		case Opcode::INSN_OPCODE_JMP_GREATER_EQUAL:	{return FLAGS_MASK_GT | FLAGS_MASK_EQ;}
		case Opcode::INSN_OPCODE_JMP_LOWER_EQUAL:	{return FLAGS_MASK_LT | FLAGS_MASK_EQ;}
		default: {break;}
		}
		assert(false);
		return FLAGS_MASK_GT;
	}

	ReturnObject DecodeStage::run()
	{
		DecodeStageValue value0;
		VectorValue value1;
		VectorValue value2;

		while (1)
		{
			if (fetch_bus.is_busy)
			{
				const FetchToDecodeBusPacket pkt = fetch_bus.recv();

				$display("DECODE[%d] exec: %x", pkt.PC, to_string(static_cast<Opcode>(pkt.insn & 0xff)));

				switch (static_cast<Opcode>(pkt.insn & 0xff))
				{
				case Opcode::INSN_OPCODE_NOP:
				{
					break;
				}

				case Opcode::INSN_OPCODE_HALT:
				{
					execute_bus.send_req1(pkt.exec_mask, pkt.PC, EXEC_HALT, value0 );
					break;
				}

				case Opcode::INSN_OPCODE_MOVE_PCREL_REG_CONST16:
				{
					value0.regID = static_cast<RegisterID>((pkt.insn >> 8) & 0xff);

					value1 = create_vec_int64(static_cast<int64_t>((pkt.insn >> 16) & 0xffff) + pkt.PC);

					execute_bus.send_req2(pkt.exec_mask, 
									pkt.PC, 
									EXEC_MOVE_REG_VALUE,
									value0,
									value1);
					break;
				}

				// reg = [reg + const]
				case Opcode::INSN_OPCODE_STORE_REG_CONST_REG:
				{
					const auto& src_reg = static_cast<RegisterID>((pkt.insn >> 8) & 0xff);
					const auto& base_reg_id = static_cast<RegisterID>((pkt.insn >> 16) & 0xff);
					const auto& addr_offset_value = static_cast<int16_t>((pkt.insn >> 24) & 0xff);
					

					value0.vec = regs[src_reg];
					value1 = create_vec_int64(addr_offset_value);
					value2 = regs[base_reg_id];

					execute_bus.send_req3(pkt.exec_mask, pkt.PC, ExecuteStageOpcode::EXEC_STORE_ADDR_VALUE,
						value0, value1, value2 );
					break;
				}

				// reg = blockIndex
				case Opcode::INSN_OPCODE_MOVE_REG_BLOCK_INDEX:
				{
					value0.regID = static_cast<RegisterID>((pkt.insn >> 8) & 0xff);
					value1 = create_vec_incrementing_values();

					execute_bus.send_req2(pkt.exec_mask, pkt.PC, ExecuteStageOpcode::EXEC_MOVE_REG_VALUE,
						value0, value1);
					break;
				}

				case Opcode::INSN_OPCODE_MOVE_REG_CONST16:
				{
					value0.regID = static_cast<RegisterID>((pkt.insn >> 8) & 0xff);
					const auto& const_value = static_cast<int16_t>((pkt.insn >> 16) & 0xffff);

					value1 = create_vec_int64(const_value);

					execute_bus.send_req2(pkt.exec_mask,  pkt.PC, ExecuteStageOpcode::EXEC_MOVE_REG_VALUE,
						value0, value1);
					break;
				}

				case Opcode::INSN_OPCODE_MOVE_REG_REG:
				{
					value0.regID = static_cast<RegisterID>((pkt.insn >> 8) & 0xff);
					const auto src_reg = static_cast<RegisterID>((pkt.insn >> 16) & 0xff);

					const auto& value = regs[src_reg];

					execute_bus.send_req2(pkt.exec_mask, pkt.PC, ExecuteStageOpcode::EXEC_MOVE_REG_VALUE,
						value0, value );
					break;
				}

				case Opcode::INSN_OPCODE_L_SSHIFT_REG_REG_CONST:
				{
					value0.regID = static_cast<RegisterID>((pkt.insn >> 8) & 0xff);
					const auto src1_reg = static_cast<RegisterID>((pkt.insn >> 16) & 0xff);
					const auto value2_const = static_cast<int8_t>((pkt.insn >> 24) & 0xff);

					value2 = create_vec_int64(value2_const);
					value1 = regs[src1_reg];

					execute_bus.send_req3(pkt.exec_mask, pkt.PC, ExecuteStageOpcode::EXEC_SHL_REG_VALUE_VALUE,
						value0, value1, value2);
					break;
				}


				case Opcode::INSN_OPCODE_ADD_REG_REG_REG:
				{
					value0.regID = static_cast<RegisterID>((pkt.insn >> 8) & 0xff);
					const auto src1_reg = static_cast<RegisterID>((pkt.insn >> 16) & 0xff);
					const auto src2_reg = static_cast<RegisterID>((pkt.insn >> 24) & 0xff);

					const auto& value1 = regs[src1_reg];
					const auto& value2 = regs[src2_reg];

					execute_bus.send_req3(pkt.exec_mask, pkt.PC, ExecuteStageOpcode::EXEC_ADD_REG_VALUE_VALUE,
						value0, value1, value2);
					break;
				}


				// reg = reg + const
				case Opcode::INSN_OPCODE_ADD_REG_REG_CONST:
				{
					value0.regID = static_cast<RegisterID>((pkt.insn >> 8) & 0xff);
					const auto src1_reg = static_cast<RegisterID>((pkt.insn >> 16) & 0xff);
					const auto value2_const = static_cast<int8_t>((pkt.insn >> 24) & 0xff);

					value2 = create_vec_int64(value2_const);
					value1 = regs[src1_reg];

					execute_bus.send_req3(pkt.exec_mask, pkt.PC, ExecuteStageOpcode::EXEC_ADD_REG_VALUE_VALUE,
						value0, 
						value1, 
						value2);
					break;
				}

				case Opcode::INSN_OPCODE_JMP_ALWAYS:
				{
					const auto off = static_cast<int32_t>(pkt.insn >> 8);

					VectorValue v;
					v.setPC(off);

					value0.vec = v;

					execute_bus.send_req1(pkt.exec_mask, pkt.PC, ExecuteStageOpcode::EXEC_JMP,
						value0);
					break;
				}

				case Opcode::INSN_OPCODE_CMP_REG_REG:
				{
					const auto reg1 = static_cast<RegisterID>((pkt.insn >> 8) & 0xff);
					const auto reg2 = static_cast<RegisterID>((pkt.insn >> 16) & 0xff);

					value0.vec = regs[reg1];
					value1 = regs[reg2];

					//std::cerr << "[DECODE] CMP: " << value1 << " -- " << value2 << std::endl;

					execute_bus.send_req2( pkt.exec_mask, pkt.PC, ExecuteStageOpcode::EXEC_CMP,
						value0,
						value1);
					break;
				}

				case Opcode::INSN_OPCODE_JMP_LOWER:
				case Opcode::INSN_OPCODE_JMP_LOWER_EQUAL:
				case Opcode::INSN_OPCODE_JMP_EQUAL:
				case Opcode::INSN_OPCODE_JMP_NOT_EQUAL:
				case Opcode::INSN_OPCODE_JMP_GREATER:
				case Opcode::INSN_OPCODE_JMP_GREATER_EQUAL:
				{
					const auto expected_mask = get_expected_cond_jump_flags_mask((Opcode)(pkt.insn & 0xff));
					value1 = create_vec_int64(expected_mask);
					value0.vec = create_vec_int64(static_cast<int32_t>(pkt.insn >> 8));

					while (execute_bus.is_busy)
					{
						CONTEXT_SWITCH();
					}
					execute_bus.send_req2(pkt.exec_mask, pkt.PC,
						ExecuteStageOpcode::EXEC_COND_JMP,
						value0, value1);
					break;
				}


				case Opcode::INSN_OPCODE_LOAD_RESTORE_PC:
				{
					const auto base = static_cast<RegisterID>((pkt.insn >> 8) & 0xff);
					const auto off1_const = static_cast<int16_t>(pkt.insn >> 16);

					value0.vec = create_vec_int64(off1_const);
					value1 = regs[base];

					execute_bus.send_req2(pkt.exec_mask, pkt.PC, ExecuteStageOpcode::EXEC_LOAD_RESTORE_PC,
							value0,
							value1);
					break;
				}

				// ret = [const + reg]
				case Opcode::INSN_OPCODE_LOAD_REG_CONST_REG:
				{
					value0.regID = static_cast<RegisterID>((pkt.insn >> 8) & 0xff);
					const auto base = static_cast<RegisterID>((pkt.insn >> 16) & 0xff);
					const auto off1_const = static_cast<int16_t>(pkt.insn >> 24);

					VectorValue off1 = create_vec_int64(off1_const);

					const auto& off2 = regs[base];

					execute_bus.send_req3(pkt.exec_mask, pkt.PC, ExecuteStageOpcode::EXEC_LOAD_REG,
							value0,
							off1,
							off2);
					break;
				}

				case Opcode::INSN_OPCODE_MOVE_R0_CONST24A:
				{
					value0.regID = RegisterID::REG_R0;
					value1 = create_vec_int64(static_cast<int32_t>(pkt.insn >> 8));

					execute_bus.send_req2(pkt.exec_mask, pkt.PC, ExecuteStageOpcode::EXEC_MOVE_REG_VALUE,
							value0,
							value1);
					break;
				}

				case Opcode::INSN_OPCODE_MOVE_R0_CONST24B:
				{
					value0.regID = RegisterID::REG_R0;
					value1 = create_vec_int64(static_cast<int32_t>(pkt.insn >> 8));
					value2 = regs[RegisterID::REG_R0];

					execute_bus.send_req3(pkt.exec_mask, pkt.PC, ExecuteStageOpcode::EXEC_ORB_REG_VALUE,
							value0,
							value1,
							value2);
					break;
				}

				case Opcode::INSN_OPCODE_MOVE_R0_CONST24C:
				{
					value0.regID = RegisterID::REG_R0;
					value1 = create_vec_int64(static_cast<int32_t>(pkt.insn >> 8));
					value2 = regs[RegisterID::REG_R0];

					execute_bus.send_req3(pkt.exec_mask, pkt.PC, 
								ExecuteStageOpcode::EXEC_ORC_REG_VALUE,
							value0,
							value1,
							value2);
					break;
				}

				default:
					$display("[DECODE] unimplemented opcode: " + to_string((Opcode)(pkt.insn & 0xff)));
					assert(false);
				}
			}


			CONTEXT_SWITCH();
		}
	}

}