interface DecodeToExecuteBus;
	bool is_busy;
	DecodeExecPacket data;
	
	
	function void init();
	begin
	begin
		is_busy = 0;
	end
	end
	endfunction
	
	
	function void send_req1(input execution_mask_t exec_mask, input memory_address_t PC, input ExecuteStageOpcode opcode, input DecodeStageValue value0);
	begin
	begin
		assert(!(is_busy));
		data.exec_mask <= exec_mask;
		data.PC <= PC;
		data.opcode <= opcode;
		data.value0 <= value0;
		is_busy <= 1;
	end
	end
	endfunction
	
	
	function void send_req2(input execution_mask_t exec_mask, input memory_address_t PC, input ExecuteStageOpcode opcode, input DecodeStageValue value0, input VectorValue value1);
	begin
	begin
		assert(!(is_busy));
		data.exec_mask <= exec_mask;
		data.PC <= PC;
		data.opcode <= opcode;
		data.value0 <= value0;
		data.value1 <= value1;
		is_busy <= 1;
	end
	end
	endfunction
	
	
	function void send_req3(input execution_mask_t exec_mask, input memory_address_t PC, input ExecuteStageOpcode opcode, input DecodeStageValue value0, input VectorValue value1, input VectorValue value2);
	begin
	begin
		assert(!(is_busy));
		data.exec_mask <= exec_mask;
		data.PC <= PC;
		data.opcode <= opcode;
		data.value0 <= value0;
		data.value1 <= value1;
		data.value2 <= value2;
		is_busy <= 1;
	end
	end
	endfunction
	
	
	function DecodeExecPacket recv();
	begin
	begin
		is_busy <= 0;
		return data;
	end
	end
	endfunction
endinterface;
