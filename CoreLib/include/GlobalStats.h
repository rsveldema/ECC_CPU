#pragma once

#include <cstdint>
#include <string>

#include "units.h"
#include "basic_display.h"

namespace ecc
{

	INTERFACE GlobalStats
	{
		uint64_t waitForInsnFetch;

		uint64_t numCyclesUsed;

		uint64_t waitForOperandFetch;

		uint64_t numVectorLocalDivergences;

		uint64_t numL1InsnCacheHits;

		METHOD_SECTION;

		void dump()
		{
			$display("-----------------------------------------");
			$display("num-cycles: ", numCyclesUsed);
			$display("wait-for-insn-fetch", waitForInsnFetch);
			$display("wait-for-op-fetch", waitForOperandFetch);
			$display("num-vec-local-divergences", numVectorLocalDivergences);
			$display("num-l1-insn-cache-hits", numL1InsnCacheHits);
		}

		void incnumL1InsnCacheHits()
		{
			numL1InsnCacheHits += 1;
		}

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

		void incFetchedInsnWait()
		{
			waitForInsnFetch++;
		}

		void init()
		{
			waitForInsnFetch = 0;
			numCyclesUsed = 0;
			waitForOperandFetch = 0;
			numVectorLocalDivergences = 0;	
			numL1InsnCacheHits = 0;
		}

	};


}