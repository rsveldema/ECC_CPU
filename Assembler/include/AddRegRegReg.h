#pragma once

#include "Instruction.h"
#include <map>

namespace Insns
{
	class AddRegRegReg : public Instruction
	{
	private:
		MachineInfo::RegisterID dst;
		MachineInfo::RegisterID src1;
		MachineInfo::RegisterID src2;

	public:
		AddRegRegReg(MachineInfo::RegisterID dst, MachineInfo::RegisterID src1, MachineInfo::RegisterID src2)
		{
			this->dst = dst;
			this->src1 = src1;
			this->src2 = src2;
		}

		uint32_t getEncodedInstruction() override
		{
			return ((uint32_t)MachineInfo::Opcode::ADD_REG_REG_REG) | ((uint32_t)dst << 8) | ((uint32_t)src1 << 16) | ((uint32_t)src2 << 24);
		}


		void link(const Linker* linker) override { }
	};
}