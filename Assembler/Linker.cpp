#include "Linker.h"


void Linker::link(std::vector<Instruction*>& insns)
{
	std::map<std::string, Instruction*> dict;

	int address = 0;
	for (auto* insn : insns)
	{
		insn->address = address;

		if (insn->label != "")
		{
			dict[insn->label] = insn;
		}

		address += 4;
	}


	for (auto* insn : insns)
	{
		insn->link(dict);
	}
}