interface GlobalStats;
	uint64_t waitForInsnFetch;
	uint64_t numCyclesUsed;
	uint64_t waitForOperandFetch;
	uint64_t numVectorLocalDivergences;
	uint64_t numL1InsnCacheHits;
	
	
	function void dump();
	begin
	begin
		$display("-----------------------------------------");
		$display("num-cycles: ", numCyclesUsed);
		$display("wait-for-insn-fetch", waitForInsnFetch);
		$display("wait-for-op-fetch", waitForOperandFetch);
		$display("num-vec-local-divergences", numVectorLocalDivergences);
		$display("num-l1-insn-cache-hits", numL1InsnCacheHits);
	end
	end
	endfunction
	
	
	function void incnumL1InsnCacheHits();
	begin
	begin
		numL1InsnCacheHits <= numL1InsnCacheHits + 1;
	end
	end
	endfunction
	
	
	function void incNumVectorLocalDivergences();
	begin
	begin
		numVectorLocalDivergences <= numVectorLocalDivergences + 1;
	end
	end
	endfunction
	
	
	function void incNumCyclesUsed();
	begin
	begin
		numCyclesUsed <= numCyclesUsed + 1;
	end
	end
	endfunction
	
	
	function void incWaitForOperandFetch();
	begin
	begin
		waitForOperandFetch <= waitForOperandFetch + 1;
	end
	end
	endfunction
	
	
	function void incFetchedInsnWait();
	begin
	begin
		waitForInsnFetch <= (waitForInsnFetch + 1);
	end
	end
	endfunction
	
	
	function void init();
	begin
	begin
		waitForInsnFetch = 0;
		numCyclesUsed = 0;
		waitForOperandFetch = 0;
		numVectorLocalDivergences = 0;
		numL1InsnCacheHits = 0;
	end
	end
	endfunction
endinterface;
