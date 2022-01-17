#pragma once

#include "Instruction.h"

namespace Insns
{
	class Halt : public Instruction
	{
	public:
		void link(const std::map<std::string, Instruction*>& dict) override {}

		uint32_t getEncodedInstruction() override
		{
			return (uint32_t)MachineInfo::Opcode::HALT;
		}
	};
}