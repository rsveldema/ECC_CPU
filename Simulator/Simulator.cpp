// ECC_CPU.cpp : Defines the entry point for the application.
//

#include "Simulator.h"

#include <fstream>

using namespace Simulator;

void Usage()
{
	std::cerr << "USAGE: sim <memorydump.bin> <num_cores>" << std::endl;
}


void read_memory_dump(CoreClusterGrid& machine, const std::string& filename)
{
	if (! filename.ends_with(".bin"))
	{
		std::cerr << "expected a .bin file"  << std::endl;
		Usage();
		return;
	}

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
	if (argc != 3)
	{
		Usage();
		return -1;
	}

	std::string filename(argv[1]);

	unsigned num_cores = std::stoi(argv[2]);

	std::cerr << "starting simulation: " << filename << std::endl;

	GlobalStats globalStats;

	MachineConfig config{
		.grid_mem_config {
			.size = 1024 * 1024,
			.read_latency = Cycles(100),
			.write_latency = Cycles(100),
		},
		.sram_config {
			.size = 1024,
			.read_latency = Cycles(10),
			.write_latency = Cycles(10),
		},
		.num_cores = num_cores
	};

	SimComponentRegistry registry(globalStats);
	CoreClusterGrid machine(registry, config, globalStats);

	read_memory_dump(machine, filename);

	registry.run(machine);

	std::cerr << "finished simulation" << std::endl;
	globalStats.dump();

	return 0;
}
