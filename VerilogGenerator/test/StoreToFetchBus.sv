interface StoreToFetchBus;
	bool can_receive = false;
	StoreToFetchPacket data;
	
	
	function void send(StoreToFetchPacket pkt);
	begin
		// assert(!(can_receive))
		data=pkt;
		can_receive=1;
	end
	endfunction
	
	
	function StoreToFetchPacket recv();
		StoreToFetchPacket v = 0;
	begin
		// assert(can_receive)
		v = data;
		can_receive=0;
		return v;
	end
	endfunction
endinterface;
