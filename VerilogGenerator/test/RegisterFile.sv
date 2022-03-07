
typedef uint8_t TinyCounter;


typedef struct packed {
	TinyCounter[((int'(MAX_REG_ID))) - 1:0] invalidated_regs;
	TinyCounter[((int'(MAX_REG_ID))) - 1:0] written_regs;
	VectorValue[((int'(MAX_REG_ID))) - 1:0] regs;
	flags_reg_t machine_flags;
} RegisterSet;
interface RegisterFile;
	RegisterSet data;
	
	
	function void init();
	begin
	begin
		data.machine_flags = 0;
	end
	end
	endfunction
	
	
	function void mark_invalid(input RegisterID id);
	begin
	begin
		assert(isValidIndex(id));
		data.invalidated_regs[(int'(id))] <= data.invalidated_regs[(int'(id))] + 1;
	end
	end
	endfunction
	
	
	function void mark_valid(input RegisterID id);
	begin
	begin
		assert(isValidIndex(id));
		data.written_regs[(int'(id))] <= data.written_regs[(int'(id))] + 1;
	end
	end
	endfunction
	
	
	function bool is_valid(input RegisterID id);
	begin
	begin
		assert(isValidIndex(id));
		return (data.written_regs[(int'(id))] == data.invalidated_regs[(int'(id))]);
	end
	end
	endfunction
	
	
	function void set(input RegisterID id, input VectorValue v);
	begin
	begin
		assert(isValidIndex(id));
		data.regs[(int'(id))] <= v;
	end
	end
	endfunction
	
	
	function VectorValue get(input RegisterID id);
	begin
	begin
		assert(isValidIndex(id));
		return data.regs[(int'(id))];
	end
	end
	endfunction
	
	
	function void setMachineFlag(input uint64_t flags);
	begin
	begin
		data.machine_flags <= data.machine_flags | flags;
	end
	end
	endfunction
	
	
	function void setHasHalted();
	begin
	begin
		data.machine_flags <= data.machine_flags | MACHINE_FLAGS_MASK_HALT;
		$finish();
	end
	end
	endfunction
	
	
	function bool hasHalted();
	begin
	begin
		return ((data.machine_flags & MACHINE_FLAGS_MASK_HALT) != 0);
	end
	end
	endfunction
endinterface;
