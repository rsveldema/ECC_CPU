#pragma once

#include "Instruction.h"
#include <map>


namespace Insns
{
	class StoreRegister : public Instruction
	{
	private:
		int offset;
		ecc::RegisterID base_reg;
		ecc::RegisterID src_reg;

	public:
		StoreRegister(ecc::RegisterID base_reg, int offset, ecc::RegisterID src_reg)
		{
			this->base_reg = base_reg;
			this->src_reg = src_reg;
			this->offset = offset;
		}

		uint32_t getEncodedInstruction() override
		{
			return (uint32_t)(((uint32_t)ecc::Opcode::STORE_REG_CONST_REG) | ((uint32_t)src_reg << 8) | ((uint32_t)base_reg << 16) | ((uint32_t)offset << 24));
		}


		void link(const Linker* linker) override
		{ }
	};
}