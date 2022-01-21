#pragma once

#include "Instruction.h"
#include <map>

namespace Insns
{
	class LoadRegister : public Instruction
	{
	private:
		MachineInfo::RegisterID dest;
		MachineInfo::RegisterID base;
		int16_t offset;

	public:
		LoadRegister(MachineInfo::RegisterID dest, MachineInfo::RegisterID base, int16_t offset)
			: dest(dest), base(base), offset(offset)
		{
		}

		uint32_t getEncodedInstruction() override
		{
			return ((uint32_t)MachineInfo::Opcode::LOAD_REG_CONST_REG) | ((uint32_t)dest << 8) | ((uint32_t)base << 16) | ((uint32_t)offset << 24);
		}

		void link(const Linker* linker) override
		{ }
	};
}