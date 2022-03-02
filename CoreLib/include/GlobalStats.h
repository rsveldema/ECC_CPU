#pragma once

#include <cstdint>
#include <string>
#include "units.h"

namespace ecc
{

	INTERFACE GlobalStats
	{
		uint64_t waitForInsnFetch;

		uint64_t numCyclesUsed;

		uint64_t waitForOperandFetch;

		uint64_t numVectorLocalDivergences;

		METHOD_SECTION;

		void incNumVectorLocalDivergences()
		{
			numVectorLocalDivergences += 1;	
		}

		void incNumCyclesUsed()
		{
			numCyclesUsed += 1;
		}

		void incWaitForOperandFetch()
		{
			waitForOperandFetch += 1;
		}

		void init()
		{
			waitForInsnFetch = 0;
			numCyclesUsed = 0;
			waitForOperandFetch = 0;
			numVectorLocalDivergences = 0;	
		}

		void incFetchedInsnWait()
		{
			waitForInsnFetch++;
		}
	};


}