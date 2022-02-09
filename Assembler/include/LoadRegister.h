#pragma once

#include "Instruction.h"
#include <map>

namespace Insns
{
	class LoadRegister : public Instruction
	{
	private:
		ecc::RegisterID dest;
		ecc::RegisterID base;
		int16_t offset;

	public:
		LoadRegister(ecc::RegisterID dest, ecc::RegisterID base, int16_t offset)
			: dest(dest), base(base), offset(offset)
		{
		}

		uint32_t getEncodedInstruction() override
		{
			return ((uint32_t)ecc::Opcode::INSN_OPCODE_LOAD_REG_CONST_REG) | ((uint32_t)dest << 8) | ((uint32_t)base << 16) | ((uint32_t)offset << 24);
		}

		void link(const Linker* linker) override
		{ }
	};
}