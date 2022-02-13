interface FetchToDecodeBus;
	bool is_busy;
	FetchToDecodeBusPacket data;
	
	
	function void init();
	begin
	begin
		is_busy = 0;
	end
	end
	endfunction
	
	
	function void send(input FetchToDecodeBusPacket pkt);
	begin
	begin
		assert(!(is_busy));
		data <= pkt;
		is_busy <= 1;
	end
	end
	endfunction
	
	
	function FetchToDecodeBusPacket recv();
	begin
		FetchToDecodeBusPacket v;
	begin
		assert(is_busy);
		v <= data;
		is_busy <= 0;
		return v;
	end
	end
	endfunction
endinterface;
