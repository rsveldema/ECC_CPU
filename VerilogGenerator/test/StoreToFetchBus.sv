interface StoreToFetchBus;
	bool can_receive;
	StoreToFetchPacket data;
	
	
	function void init();
	begin
	begin
		can_receive = 0;
	end
	end
	endfunction
	
	
	function void send(input execution_mask_t mask, input memory_address_t new_pc);
	begin
	begin
		assert(!(can_receive));
		data.exec_mask <= mask;
		data.newpc <= new_pc;
		can_receive <= 1;
	end
	end
	endfunction
	
	
	function StoreToFetchPacket recv();
	begin
	begin
		assert(can_receive);
		can_receive <= 0;
		return data;
	end
	end
	endfunction
endinterface;
