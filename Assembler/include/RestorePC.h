#pragma once

#include "Instruction.h"
#include <map>

namespace Insns
{
	class RestorePC : public Instruction
	{
	private:
		int16_t offset;
		MachineInfo::RegisterID reg;

	public:
		RestorePC(MachineInfo::RegisterID reg, int16_t offset)
		{
			this->reg = reg;
			this->offset = offset;
		}

		uint32_t getEncodedInstruction() override
		{
			return ((uint32_t)MachineInfo::Opcode::LOAD_RESTORE_PC) | ((uint32_t)reg << 8) | ((uint32_t)offset << 16);
		}


		void link(const std::map<std::string, Instruction*>& dict) override
		{ }
	};
}