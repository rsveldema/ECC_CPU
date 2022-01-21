#pragma once

#include "Instruction.h"

#include <map>

class Linker;

namespace Insns
{
	class Jmp : public Instruction
	{
	private:
		const SourcePosition pos;
		std::string label;
		int64_t dest = 0;
		Mnemonic mnemonic;

	public:
		Jmp(const std::string& label, const SourcePosition& pos, Mnemonic _mnemonic)
			: pos(pos), label(label), mnemonic(_mnemonic)
		{
		}

		uint32_t getEncodedInstruction() override
		{
			auto op = MachineInfo::Opcode::JMP_ALWAYS;

			switch (mnemonic)
			{
			case Mnemonic::JMP: op = MachineInfo::Opcode::JMP_ALWAYS; break;
			case Mnemonic::JE: op = MachineInfo::Opcode::JMP_EQUAL; break;
			case Mnemonic::JNE: op = MachineInfo::Opcode::JMP_NOT_EQUAL; break;
			case Mnemonic::JL: op = MachineInfo::Opcode::JMP_LOWER; break;
			case Mnemonic::JLE: op = MachineInfo::Opcode::JMP_LOWER_EQUAL; break;
			case Mnemonic::JG: op = MachineInfo::Opcode::JMP_GREATER; break;
			case Mnemonic::JGE: op = MachineInfo::Opcode::JMP_GREATER_EQUAL; break;
			default:
			{
				abort();
				break;
			}
			}

			return (uint32_t)(((uint32_t)op) | ((uint32_t)dest << 8));
		}

		void link(const Linker* linker) override;
	};
}