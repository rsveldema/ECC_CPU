#pragma once

#include <cstdint>
#include <string>
#include "units.h"

struct GlobalStats
{
	Cycles waitForInsnFetch;

	Cycles numCyclesUsed;


	void dump()
	{
		std::cout << "TOOK " << std::to_string(numCyclesUsed.cycles) << std::endl;
		std::cout << "#wait cycles in fetch-insn: " << std::to_string(waitForInsnFetch.cycles) << std::endl;
	}
};