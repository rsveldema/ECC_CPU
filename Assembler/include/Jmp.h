#pragma once

#include <map>

#include "Instruction.h"

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
			auto op = ecc::Opcode::INSN_OPCODE_JMP_ALWAYS;

			switch (mnemonic)
			{
			case Mnemonic::JMP: op = ecc::Opcode::INSN_OPCODE_JMP_ALWAYS; break;
			case Mnemonic::JE: op = ecc::Opcode::INSN_OPCODE_JMP_EQUAL; break;
			case Mnemonic::JNE: op = ecc::Opcode::INSN_OPCODE_JMP_NOT_EQUAL; break;
			case Mnemonic::JL: op = ecc::Opcode::INSN_OPCODE_JMP_LOWER; break;
			case Mnemonic::JLE: op = ecc::Opcode::INSN_OPCODE_JMP_LOWER_EQUAL; break;
			case Mnemonic::JG: op = ecc::Opcode::INSN_OPCODE_JMP_GREATER; break;
			case Mnemonic::JGE: op = ecc::Opcode::INSN_OPCODE_JMP_GREATER_EQUAL; break;
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