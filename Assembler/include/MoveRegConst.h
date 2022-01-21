#pragma once

#include "Instruction.h"
#include <map>

namespace Insns
{
	class MoveRegConst : public Instruction
	{
	private:
		MachineInfo::RegisterID dst;
		int16_t src;

	public:
		MoveRegConst(MachineInfo::RegisterID dst, int16_t src2)
		{
			this->dst = dst;
			this->src = src2;
		}

		uint32_t getEncodedInstruction() override
		{
			return ((uint32_t)MachineInfo::Opcode::MOVE_REG_CONST16) | ((uint32_t)dst << 8) | ((uint32_t)src << 16);
		}


		void link(const Linker* linker) override
		{ }
	};
}