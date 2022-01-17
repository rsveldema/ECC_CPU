#pragma once

#include "Instruction.h"
#include <map>

namespace Insns
{
	class AddRegRegReg : public Instruction
	{
	private:
		MachineInfo::Register dst;
		MachineInfo::Register src1;
		MachineInfo::Register src2;

	public:
		AddRegRegReg(MachineInfo::Register dst, MachineInfo::Register src1, MachineInfo::Register src2)
		{
			this->dst = dst;
			this->src1 = src1;
			this->src2 = src2;
		}

		uint32_t getEncodedInstruction() override
		{
			return ((uint32_t)MachineInfo::Opcode::ADD64_REG_REG_REG) | ((uint32_t)dst << 8) | ((uint32_t)src1 << 16) | ((uint32_t)src2 << 24);
		}


		void link(const std::map<std::string, Instruction*>& dict) override
		{ }
	};
}