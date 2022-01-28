#pragma once

#include "Instruction.h"
#include <map>

namespace Insns
{
	class MoveRegReg : public Instruction
	{
	private:
		ecc::RegisterID dst;
		ecc::RegisterID src;

	public:
		MoveRegReg(ecc::RegisterID dst, ecc::RegisterID src)
		{
			this->dst = dst;
			this->src = src;
		}

		uint32_t getEncodedInstruction() override
		{
			return ((uint32_t)ecc::Opcode::MOVE_REG_REG) | ((uint32_t)dst << 8) | ((uint32_t)src << 16);
		}


		void link(const Linker* linker) override
		{ }
	};
}