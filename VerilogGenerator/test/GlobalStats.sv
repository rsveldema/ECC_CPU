interface GlobalStats;
	uint64_t waitForInsnFetch;
	uint64_t numCyclesUsed;
	uint64_t waitForOperandFetch;
	uint64_t numVectorLocalDivergences;
	
	
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
	
	
	function void init();
	begin
	begin
		waitForInsnFetch = 0;
		numCyclesUsed = 0;
		waitForOperandFetch = 0;
		numVectorLocalDivergences = 0;
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
endinterface;
