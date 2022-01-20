#pragma once

#include "Instruction.h"
#include <map>

namespace Insns
{
	class MoveRegBlockIndex : public Instruction
	{
	private:
		MachineInfo::RegisterID dst;
		MachineInfo::RegisterID src;

	public:
		MoveRegBlockIndex(MachineInfo::RegisterID dst, MachineInfo::RegisterID src)
		{
			this->dst = dst;
			this->src = src;
		}

		uint32_t getEncodedInstruction() override
		{
			return ((uint32_t)MachineInfo::Opcode::MOVE_REG_BLOCK_INDEX) | ((uint32_t)dst << 8) | ((uint32_t)src << 16);
		}


		void link(const std::map<std::string, Instruction*>& dict) override
		{ }
	};
}