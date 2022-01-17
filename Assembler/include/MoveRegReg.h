#pragma once

#include "Instruction.h"
#include <map>

namespace Insns
{
	class MoveRegReg : public Instruction
	{
	private:
		MachineInfo::Register dst;
		MachineInfo::Register src;

	public:
		MoveRegReg(MachineInfo::Register dst, MachineInfo::Register src)
		{
			this->dst = dst;
			this->src = src;
		}

		uint32_t getEncodedInstruction() override
		{
			return ((uint32_t)MachineInfo::Opcode::MOVE_REG_REG) | ((uint32_t)dst << 8) | ((uint32_t)src << 16);
		}


		void link(const std::map<std::string, Instruction*>& dict) override
		{ }
	};
}