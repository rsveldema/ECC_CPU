interface StoreToFetchBus;
	bool can_receive;
	StoreToFetchPacket data;
	
	
	function void init_store_to_fetch_bus();
	begin
		can_receive=0;
	end
	endfunction
	
	
	function void send(StoreToFetchPacket pkt);
	begin
		assert(!(can_receive));
		data=pkt;
		can_receive=1;
	end
	endfunction
	
	
	function StoreToFetchPacket recv();
		StoreToFetchPacket v;
	begin
		assert(can_receive);
		v = data;
		can_receive=0;
		return v;
	end
	endfunction
endinterface;
