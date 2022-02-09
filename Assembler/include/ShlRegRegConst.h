#pragma once

#include "Instruction.h"
#include <map>

namespace Insns
{
	class ShlRegRegConst : public Instruction
	{
	private:
		ecc::RegisterID dst;
		ecc::RegisterID src;
		int8_t bits;

	public:
		ShlRegRegConst(ecc::RegisterID dst, ecc::RegisterID src, int8_t bits)
		{
			this->dst = dst;
			this->src = src;
			this->bits = bits;
		}

		uint32_t getEncodedInstruction() override
		{
			return ((uint32_t)ecc::Opcode::INSN_OPCODE_L_SSHIFT_REG_REG_CONST) | ((uint32_t)dst << 8) | ((uint32_t)src << 16) | ((uint32_t)bits << 24);
		}


		void link(const Linker* linker) override
		{ }
	};
}