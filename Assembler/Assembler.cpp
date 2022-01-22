#include "Assembler.h"
#include <iostream>
#include <filesystem>
#include "Linker.h"
#include "ObjectWriter.h"
#include "Instruction.h"

int errorCount;


namespace Insns
{
	void LoadAddr::link(const Linker* linker)
	{
		auto data = linker->find_data(this->global_var_name);
		if (!data)
		{
			pos.error("failed to find " + this->global_var_name);
			return;
		}
		auto val = *data;
		this->address = val->getAddress();
	}

	void Jmp::link(const Linker* linker)
	{
		auto insn = linker->find_insn(this->label);
		if (!insn)
		{
			pos.error("failed to find " + this->label);
			return;
		}
		auto* k = *insn;
		dest = k->address - this->address;
	}

	void MoveRegSymbol::link(const Linker* linker)
	{
		auto insn = linker->find_insn(this->symbol);
		if (!insn)
		{
			pos.error("failed to resolve label in move-reg-sym: '" + this->symbol + "'");
			return;
		}
		auto* k = *insn;
		src = k->address - this->address;
	}
}



void Usage()
{
	fprintf(stderr, "asm <infile.asm> <outfile.bin>\n");
}


int main(int argc, char** argv)
{
	if (argc != 3)
	{
		Usage();
		return 1;
	}

	std::string infile = argv[1];
	std::string outfile = argv[2];

	auto dataFile = outfile + ".data";

	std::cout << "asm: " << std::filesystem::current_path() << std::endl;
	std::cout << "parsing " << infile << " writing output to " << outfile << std::endl;

	Program program;
	program.parse(infile);

	Linker linker;
	linker.link(program.getInsns(), program.getObjects(), MachineInfo::DATA_SEGMENT_START, MachineInfo::CODE_SEGMENT_START);

	ObjectWriter::write(program.getInsns(), outfile);

	ObjectWriter::write(program.getObjects(), dataFile);

	return 0;
}