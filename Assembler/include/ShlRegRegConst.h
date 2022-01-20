#pragma once

#include "Instruction.h"
#include <map>

namespace Insns
{
	class ShlRegRegConst : public Instruction
	{
	private:
		MachineInfo::RegisterID dst;
		MachineInfo::RegisterID src;
		int8_t bits;

	public:
		ShlRegRegConst(MachineInfo::RegisterID dst, MachineInfo::RegisterID src, int8_t bits)
		{
			this->dst = dst;
			this->src = src;
			this->bits = bits;
		}

		uint32_t getEncodedInstruction() override
		{
			return ((uint32_t)MachineInfo::Opcode::L_SSHIFT_REG_REG_CONST) | ((uint32_t)dst << 8) | ((uint32_t)src << 16) | ((uint32_t)bits << 24);
		}


		void link(const std::map<std::string, Instruction*>& dict) override
		{ }
	};
}