#pragma once

#include "Instruction.h"

#include <map>

namespace Insns
{
	class Jmp : public Instruction
	{
	private:
		const SourcePosition pos;
		std::string label;
		int64_t dest = 0;

	public:
		Jmp(const std::string& label, const SourcePosition& pos)
			: pos(pos), label(label)
		{
		}

		uint32_t getEncodedInstruction() override
		{
			return (uint32_t)(((uint32_t)MachineInfo::Opcode::JMP) | ((uint32_t)dest << 8));
		}

		void link(const std::map<std::string, Instruction*>& dict) override
		{
			auto it = dict.find(label);
			if (it == dict.end())
			{
				pos.error("failed to resolve label: '" + label + "'");
				return;
			}
			auto insn = it->second;
			dest = insn->address - this->address;
		}
	};
}