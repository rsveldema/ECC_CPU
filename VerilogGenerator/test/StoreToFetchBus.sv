interface StoreToFetchBus;
	bool can_receive = false;
	StoreToFetchPacket data;
	
	
	function send(StoreToFetchPacket pkt);
		reg[32:0] state = 0;
	begin
		// assert(!(can_receive))
		data=pkt;
		can_receive=1;
	end
	endfunction
	
	
	function recv();
		reg[32:0] state = 0;
		StoreToFetchPacket v = 0;
	begin
		// assert(can_receive)
		v = data;
		can_receive=0;
		return v;
	end
	endfunction
endinterface;
