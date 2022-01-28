#pragma once

#include "Instruction.h"
#include <map>

namespace Insns
{
	class CmpRegReg : public Instruction
	{
	private:
		ecc::RegisterID dst;
		ecc::RegisterID src;

	public:
		CmpRegReg(ecc::RegisterID _dst, ecc::RegisterID _src)
		{
			this->dst = _dst;
			this->src = _src;
		}

		uint32_t getEncodedInstruction() override
		{
			return ((uint32_t)ecc::Opcode::CMP_REG_REG) | ((uint32_t)dst << 8) | ((uint32_t)src << 16);
		}



		void link(const Linker* linker) override { }
	};
}