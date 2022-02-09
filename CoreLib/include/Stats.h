#pragma once

#include <cstdint>
#include <string>
#include "units.h"

namespace ecc
{

	struct GlobalStats
	{
		Cycles waitForInsnFetch;

		Cycles numCyclesUsed;

		Cycles waitForOperandFetch;

		uint64_t numVectorLocalDivergences = 0;

		void dump()
		{
			std::cout << "-----------------<<[  Stats ]>>------------------" << std::endl;
			std::cout << "Total #cycles                   : " << std::to_string(numCyclesUsed.cycles) << std::endl;
			std::cout << "wait #wait cycles in fetch-insn : " << std::to_string(waitForInsnFetch.cycles) << std::endl;
			std::cout << "wait #wait cycles in fetch-op   : " << std::to_string(waitForOperandFetch.cycles) << std::endl;
			std::cout << "numVectorLocalDivergences       : " << std::to_string(numVectorLocalDivergences) << std::endl;
		}
	};

	extern GlobalStats __global_stats;

	static inline void incFetchedInsnWait()
	{
		__global_stats.waitForInsnFetch++;
	}

}