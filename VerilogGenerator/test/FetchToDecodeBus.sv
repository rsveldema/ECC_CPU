interface FetchToDecodeBus;
	bool is_busy;
	FetchToDecodeBusPacket data;
	
	
	function void init_fetch_to_decode_bus();
	begin
		is_busy=0;
	end
	endfunction
	
	
	function void send(input FetchToDecodeBusPacket pkt);
	begin
		assert(!(is_busy));
		data=pkt;
		is_busy=1;
	end
	endfunction
	
	
	function FetchToDecodeBusPacket recv();
	begin
		FetchToDecodeBusPacket v;
		assert(is_busy);
		v = data;
		is_busy=0;
		return v;
	end
	endfunction
endinterface;
