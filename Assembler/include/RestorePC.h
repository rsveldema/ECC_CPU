#pragma once

#include "Instruction.h"
#include <map>

namespace Insns
{
	class RestorePC : public Instruction
	{
	private:
		int16_t offset;
		ecc::RegisterID reg;

	public:
		RestorePC(ecc::RegisterID reg, int16_t offset)
		{
			this->reg = reg;
			this->offset = offset;
		}

		uint32_t getEncodedInstruction() override
		{
			return ((uint32_t)ecc::Opcode::LOAD_RESTORE_PC) | ((uint32_t)reg << 8) | ((uint32_t)offset << 16);
		}


		void link(const Linker* linker) override
		{ }
	};
}