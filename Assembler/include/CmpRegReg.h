#pragma once

#include "Instruction.h"
#include <map>

namespace Insns
{
	class CmpRegReg : public Instruction
	{
	private:
		MachineInfo::RegisterID dst;
		MachineInfo::RegisterID src;

	public:
		CmpRegReg(MachineInfo::RegisterID dst, MachineInfo::RegisterID src)
		{
			this->dst = dst;
			this->src = src;
		}

		uint32_t getEncodedInstruction() override
		{
			return ((uint32_t)MachineInfo::Opcode::CMP_REG_REG) | ((uint32_t)dst << 8) | ((uint32_t)src << 16);
		}


		void link(const std::map<std::string, Instruction*>& dict) override
		{ }
	};
}