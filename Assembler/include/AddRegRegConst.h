#pragma once

#include "Instruction.h"
#include <map>

namespace Insns
{
	class AddRegRegConst : public Instruction
	{
	private:
		ecc::RegisterID dst;
		ecc::RegisterID src1;
		int8_t src2;

	public:
		AddRegRegConst(ecc::RegisterID dst, ecc::RegisterID src1, int8_t src2)
		{
			this->dst = dst;
			this->src1 = src1;
			this->src2 = src2;
		}

		uint32_t getEncodedInstruction() override
		{
			return ((uint32_t)ecc::Opcode::INSN_OPCODE_ADD_REG_REG_CONST) | ((uint32_t)dst << 8) | ((uint32_t)src1 << 16) | ((uint32_t)src2 << 24);
		}


		void link(const Linker* linker) override
		{ }
	};
}