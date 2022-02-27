
parameter uint32_t MAX_THREAD_CONTEXTS = 64;


typedef struct packed {
	RegisterFile regs;
	memory_address_t PC;
	execution_mask_t exec_mask;
} ThreadContext;
interface DivergenceQueue;
	ThreadContext contexts[MAX_THREAD_CONTEXTS];
	uint16_t read_pos;
	uint16_t write_pos;
	
	
	function void init();
	begin
	begin
		read_pos = 0;
		write_pos = 0;
	end
	end
	endfunction
	
	
	function bool is_empty();
	begin
	begin
		return (read_pos == write_pos);
	end
	end
	endfunction
	
	
	function void push_to_front(input RegisterFile regs, input memory_address_t PC, input execution_mask_t exec_mask);
	begin
	begin
		if ((get_next_pos(write_pos) == read_pos))
		begin
			$display("DivergenceQueue is FULL!");
			assert(0);
		end
		contexts[write_pos].exec_mask <= exec_mask;
		contexts[write_pos].PC <= PC;
		contexts[write_pos].regs <= regs;
	end
	end
	endfunction
	
	
	function ThreadContext get_back();
	begin
	begin
		assert(!(is_empty()));
		return contexts[read_pos];
	end
	end
	endfunction
	
	
	function void advance_read_pos();
	begin
	begin
		read_pos <= get_next_pos(read_pos);
	end
	end
	endfunction
	
	
	function void advance_write_pos();
	begin
	begin
		write_pos <= get_next_pos(write_pos);
	end
	end
	endfunction
	
	
	function uint32_t get_next_pos(input uint32_t v);
	begin
	begin
		if ((v == (MAX_THREAD_CONTEXTS - 1)))
		begin
			return 0;
		end
		else
		begin
			return (v + 1);
		end
	end
	end
	endfunction
endinterface;
