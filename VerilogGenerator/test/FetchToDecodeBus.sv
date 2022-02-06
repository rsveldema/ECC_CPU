interface FetchToDecodeBus;
	bool is_busy = false;
	FetchToDecodeBusPacket data;
	
	
	function void send(FetchToDecodeBusPacket pkt);
		reg[32:0] state = 0;
	begin
		// assert(!(is_busy))
		data=pkt;
		is_busy=1;
	end
	endfunction
	
	
	function FetchToDecodeBusPacket recv();
		reg[32:0] state = 0;
		FetchToDecodeBusPacket v = 0;
	begin
		// assert(is_busy)
		v = data;
		is_busy=0;
		return v;
	end
	endfunction
endinterface;
