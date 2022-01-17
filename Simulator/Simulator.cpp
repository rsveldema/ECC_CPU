// ECC_CPU.cpp : Defines the entry point for the application.
//

#include "Simulator.h"

#include <fstream>

using namespace Simulator;

void Usage()
{
	std::cerr << "USAGE: sim <memorydump.bin>" << std::endl;
}


void read_memory_dump(Machine& machine, const std::string& filename)
{
	std::fstream f;
	f.open(filename, std::ios::binary | std::ios::in);
	if (!f.is_open())
	{
		std::cerr << "failed to open " << filename << std::endl;
		return;
	}

	uint64_t address = 0;
	while (!f.eof())
	{
		char insn[4];
		f.read(insn, sizeof(insn));
		machine.dram.write(address, insn, sizeof(insn));
		address += sizeof(insn);
	}

	f.close();
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		Usage();
		return -1;
	}

	std::string filename(argv[1]);

	std::cerr << "starting simulation: " << filename << std::endl;

	SimComponentRegistry registry;
	Machine machine(registry, 1);

	read_memory_dump(machine, filename);

	registry.run(machine);

	std::cerr << "finished simulation" << std::endl;

	return 0;
}
