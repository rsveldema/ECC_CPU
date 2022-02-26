

typedef union packed {
	memory_address_t address;
	VectorValue value;
	RegisterID regID;
} ExecStageValue;


typedef struct packed {
	execution_mask_t exec_mask;
	memory_address_t PC;
	StorageStageOpcode opcode;
	ExecStageValue dest;
	ExecStageValue src;
	bool is_store_to_pc;
	execution_mask_t execution_flags_true;
	execution_mask_t execution_flags_false;
} ExecStagePacket;
interface ExecuteToStoreBus;
	bool is_busy;
	ExecStagePacket data;
	
	
	function void init();
	begin
	begin
		is_busy = 0;
	end
	end
	endfunction
	
	
	function void send_none(input execution_mask_t exec_mask, input memory_address_t PC, input StorageStageOpcode opcode);
	begin
	begin
		assert(!(is_busy));
		data.exec_mask <= exec_mask;
		data.PC <= PC;
		data.opcode <= opcode;
		data.is_store_to_pc <= 0;
		is_busy <= 1;
	end
	end
	endfunction
	
	
	function void send_reg_vec(input execution_mask_t exec_mask, input memory_address_t PC, input StorageStageOpcode opcode, input RegisterID reg_id, input VectorValue vec, input bool store_to_pc);
	begin
	begin
		assert(!(is_busy));
		data.exec_mask <= exec_mask;
		data.PC <= PC;
		data.opcode <= opcode;
		data.dest.regID <= reg_id;
		data.src.value <= vec;
		data.is_store_to_pc <= store_to_pc;
		is_busy <= 1;
	end
	end
	endfunction
	
	
	function void send_vec_vec(input execution_mask_t exec_mask, input memory_address_t PC, input StorageStageOpcode opcode, input VectorValue vec1, input VectorValue vec2);
	begin
	begin
		assert(!(is_busy));
		data.exec_mask <= exec_mask;
		data.PC <= PC;
		data.opcode <= opcode;
		data.dest.value <= vec1;
		data.src.value <= vec2;
		data.is_store_to_pc <= 0;
		is_busy <= 1;
	end
	end
	endfunction
	
	
	function void send_address(input execution_mask_t exec_mask, input memory_address_t PC, input StorageStageOpcode opcode, input memory_address_t addr);
	begin
	begin
		assert(!(is_busy));
		data.exec_mask <= exec_mask;
		data.PC <= PC;
		data.opcode <= opcode;
		data.dest.address <= addr;
		data.is_store_to_pc <= 0;
		is_busy <= 1;
	end
	end
	endfunction
	
	
	function void send_2_address(input execution_mask_t exec_mask, input memory_address_t PC, input StorageStageOpcode opcode, input memory_address_t addr1, input memory_address_t addr2, input execution_mask_t true_mask1, input execution_mask_t false_mask2);
	begin
	begin
		assert(!(is_busy));
		data.exec_mask <= exec_mask;
		data.PC <= PC;
		data.opcode <= opcode;
		data.dest.address <= addr1;
		data.src.address <= addr2;
		data.is_store_to_pc <= 0;
		data.execution_flags_true <= true_mask1;
		data.execution_flags_false <= false_mask2;
		is_busy <= 1;
	end
	end
	endfunction
	
	
	function ExecStagePacket recv();
	begin
	begin
		assert(is_busy);
		is_busy <= 0;
		return data;
	end
	end
	endfunction
endinterface;
