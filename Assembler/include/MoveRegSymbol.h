#pragma once

#include <string>

namespace Insns
{
	class MoveRegSymbol : public Instruction
	{
	private:
		const SourcePosition pos;
		MachineInfo::RegisterID dest;
		std::string symbol;
		int64_t src;

	public:
		MoveRegSymbol(MachineInfo::RegisterID dest, std::string symbol, const SourcePosition& pos)
			: pos(pos)
		{
			this->dest = dest;
			this->src = 0;
			this->symbol = symbol;
		}

		uint32_t getEncodedInstruction()override
		{
			return (uint32_t)(((uint32_t)MachineInfo::Opcode::MOVE_PCREL_REG_CONST16) | ((uint32_t)dest << 8) | ((uint32_t)src << 16));
		}

		void link(const std::map<std::string, Instruction*>& dict) override
		{
			auto it = dict.find(this->symbol);
			if (it == dict.end())
			{
				pos.error("failed to resolve label in move-reg-sym: '" + this->symbol + "'");
				return;
			}
			auto insn = it->second;
			src = insn->address - this->address;
		}
	};
}
