interface RegisterFile;
	
	typedef uint8_t TinyCounter;
	VectorValue regs[(int'(MAX_REG_ID))];
	TinyCounter invalidated_regs[(int'(MAX_REG_ID))];
	TinyCounter written_regs[(int'(MAX_REG_ID))];
	uint32_t machine_flags = 0;
	
	
	function void mark_invalid(input RegisterID id);
	begin
	begin
		assert(isValidIndex(id));
		invalidated_regs[(int'(id))] <= invalidated_regs[(int'(id))] + 1;
	end
	end
	endfunction
	
	
	function void mark_valid(input RegisterID id);
	begin
	begin
		assert(isValidIndex(id));
		written_regs[(int'(id))] <= written_regs[(int'(id))] + 1;
	end
	end
	endfunction
	
	
	function bool is_valid(input RegisterID id);
	begin
	begin
		assert(isValidIndex(id));
		return (written_regs[(int'(id))] == invalidated_regs[(int'(id))]);
	end
	end
	endfunction
	
	
	function void set(input RegisterID id, input VectorValue v);
	begin
	begin
		assert(isValidIndex(id));
		regs[(int'(id))] <= v;
	end
	end
	endfunction
	
	
	function VectorValue get(input RegisterID id);
	begin
	begin
		assert(isValidIndex(id));
		return regs[(int'(id))];
	end
	end
	endfunction
	
	
	function void setMachineFlag(input uint32_t flags);
	begin
	begin
		machine_flags <= machine_flags | flags;
	end
	end
	endfunction
	
	
	function void setHasHalted();
	begin
	begin
		machine_flags <= machine_flags | MACHINE_FLAGS_MASK_HALT;
	end
	end
	endfunction
	
	
	function bool hasHalted();
	begin
	begin
		return (machine_flags & MACHINE_FLAGS_MASK_HALT);
	end
	end
	endfunction
endinterface;
