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
	
	
	function void init_store_to_fetch_bus();
	begin
	begin
		can_receive = 0;
	end
	end
	endfunction
	
	
	function void send(input StoreToFetchPacket pkt);
	begin
	begin
		assert(!(can_receive));
		data <= pkt;
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
