#pragma once

#include <string>
#include "Instruction.h"

namespace Insns
{
	class MoveRegSymbol : public Instruction
	{
	private:
		const SourcePosition pos;
		ecc::RegisterID dest;
		std::string symbol;
		int64_t src;

	public:
		MoveRegSymbol(ecc::RegisterID dest, std::string symbol, const SourcePosition& pos)
			: pos(pos)
		{
			this->dest = dest;
			this->src = 0;
			this->symbol = symbol;
		}

		uint32_t getEncodedInstruction()override
		{
			return (uint32_t)(((uint32_t)ecc::Opcode::INSN_OPCODE_MOVE_PCREL_REG_CONST16) | ((uint32_t)dest << 8) | ((uint32_t)src << 16));
		}

		void link(const Linker* linker) override;
	};
}
