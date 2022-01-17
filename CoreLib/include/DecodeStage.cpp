#include "CoreLib.h"
#include "MachineInfo.h"


namespace Simulator
{
	coro::ReturnObject DecodeStage::run()
	{
		while (1)
		{
			if (auto pkt_opt = this->fetch_bus.try_recv())
			{
				auto pkt = *pkt_opt;

				auto PC = pkt.PC;
				auto opcode = (MachineInfo::Opcode)(pkt.insn & 0xff);

				logger.debug("DECODE[" + std::to_string(PC) + "] exec: " + MachineInfo::to_string(opcode));

				switch (opcode)
				{
				case MachineInfo::Opcode::NOP:
				{
					break;
				}
				case MachineInfo::Opcode::HALT:
				{
					regs[MachineInfo::Register::FLAGS] |= MachineInfo::REG_HALT_FLAG;
					break;
				}

				case MachineInfo::Opcode::MOVE_PCREL_REG_CONST16:
				{
					auto reg = static_cast<MachineInfo::Register>((pkt.insn >> 8) & 0xff);
					auto constValue = static_cast<int64_t>((pkt.insn >> 16) & 0xffff);

					auto value = constValue + (pkt.PC);

					DecodeToExecuteBus::Packet execute_pkt{ PC, MachineInfo::ExecuteStageOpcode::MOVE_REG_VALUE, (int64_t)reg, (int64_t)value };
					execute_bus.send(execute_pkt);
					break;
				}

				case MachineInfo::Opcode::STORE_REG_CONST_REG:
				{
					auto src_reg = static_cast<MachineInfo::Register>((pkt.insn >> 8) & 0xff);
					auto base = static_cast<MachineInfo::Register>((pkt.insn >> 16) & 0xff);
					auto addr1 = static_cast<int16_t>((pkt.insn >> 24) & 0xff);
					auto addr2 = regs[base];

					auto value = regs[src_reg];

					DecodeToExecuteBus::Packet execute_pkt{ PC, MachineInfo::ExecuteStageOpcode::STORE_ADDR_VALUE, (int64_t)value, addr1, addr2 };
					execute_bus.send(execute_pkt);
					break;
				}

				case MachineInfo::Opcode::MOVE_REG_CONST16:
				{
					auto reg = static_cast<MachineInfo::Register>((pkt.insn >> 8) & 0xff);
					auto value = static_cast<int16_t>((pkt.insn >> 16) & 0xffff);

					DecodeToExecuteBus::Packet execute_pkt{ PC, MachineInfo::ExecuteStageOpcode::MOVE_REG_VALUE, (int64_t)reg, value };
					execute_bus.send(execute_pkt);
					break;
				}

				case MachineInfo::Opcode::MOVE_REG_REG:
				{
					auto reg = static_cast<MachineInfo::Register>((pkt.insn >> 8) & 0xff);
					auto src_reg = static_cast<MachineInfo::Register>((pkt.insn >> 16) & 0xff);

					auto value = regs[src_reg];

					DecodeToExecuteBus::Packet execute_pkt{ PC, MachineInfo::ExecuteStageOpcode::MOVE_REG_VALUE, (int64_t)reg, (int64_t)value };
					execute_bus.send(execute_pkt);
					break;
				}

				case MachineInfo::Opcode::ADD64_REG_REG_REG:
				{
					auto dst_reg = static_cast<MachineInfo::Register>((pkt.insn >> 8) & 0xff);
					auto src1_reg = static_cast<MachineInfo::Register>((pkt.insn >> 16) & 0xff);
					auto src2_reg = static_cast<MachineInfo::Register>((pkt.insn >> 24) & 0xff);

					auto value1 = regs[src1_reg];
					auto value2 = regs[src2_reg];

					DecodeToExecuteBus::Packet execute_pkt{ PC, MachineInfo::ExecuteStageOpcode::MOVE_REG_VALUE, (int64_t)dst_reg, (int64_t)value1, (int64_t)value2 };
					execute_bus.send(execute_pkt);
					break;
				}

				case MachineInfo::Opcode::ADD64_REG_REG_CONST:
				{
					auto dst_reg = static_cast<MachineInfo::Register>((pkt.insn >> 8) & 0xff);
					auto src1_reg = static_cast<MachineInfo::Register>((pkt.insn >> 16) & 0xff);
					auto value2 = static_cast<int8_t>((pkt.insn >> 24) & 0xff);

					auto value1 = regs[src1_reg];

					DecodeToExecuteBus::Packet execute_pkt{ PC, MachineInfo::ExecuteStageOpcode::MOVE_REG_VALUE, (int64_t)dst_reg, (int64_t)value1, (int64_t)value2 };
					execute_bus.send(execute_pkt);
					break;
				}

				case MachineInfo::Opcode::JMP:
				{
					auto off = static_cast<int32_t>(pkt.insn >> 8);

					DecodeToExecuteBus::Packet execute_pkt{ PC, MachineInfo::ExecuteStageOpcode::JMP, (int64_t)off };
					execute_bus.send(execute_pkt);
					break;
				}

				case MachineInfo::Opcode::LOAD_RESTORE_PC:
				{
					auto base = static_cast<MachineInfo::Register>((pkt.insn >> 8) & 0xff);
					auto off1 = static_cast<int16_t>(pkt.insn >> 16);

					auto off2 = regs[base];

					DecodeToExecuteBus::Packet execute_pkt{ PC, MachineInfo::ExecuteStageOpcode::LOAD_RESTORE_PC,
							(int64_t)off1,
							(int64_t)off2,
					};
					execute_bus.send(execute_pkt);
					break;
				}


				case MachineInfo::Opcode::LOAD_REG_CONST_REG:
				{
					auto dest = static_cast<MachineInfo::Register>((pkt.insn >> 8) & 0xff);
					auto base = static_cast<MachineInfo::Register>((pkt.insn >> 16) & 0xff);
					auto off1 = static_cast<int16_t>(pkt.insn >> 24);

					auto off2 = regs[base];

					DecodeToExecuteBus::Packet execute_pkt{ PC, MachineInfo::ExecuteStageOpcode::LOAD_REG,
							(int64_t)dest,
							(int64_t)off1,
							(int64_t)off2,
					};
					execute_bus.send(execute_pkt);
					break;
				}

				default:
					std::cerr << "[DECODE] unimplemented opcode: " << MachineInfo::to_string(opcode) << std::endl;
					abort();
				}
			}
			co_await *this;
		}
	}
}