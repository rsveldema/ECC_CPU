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
	begin
		assert(is_busy);
		is_busy <= 0;
		return data;
	end
	end
	endfunction
endinterface;
