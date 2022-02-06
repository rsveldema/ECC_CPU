interface MemoryBus;
	bool request_busy = false;
	bool response_busy = false;
	BusPacket request_data;
	BusPacket response_data;
	
	
	function send_read_request_data(memory_address_t address, BusID source);
		reg[32:0] state = 0;
		BusPacket pkt = 0;
	begin
		// local_obj BusPacket pkt(BusPacketType::read_data, source, address)
		// send_request(pkt)
	end
	endfunction
	
	
	function send_write_request_data(memory_address_t address, BusID source, bus_packet_payload_t value);
		reg[32:0] state = 0;
		BusPacket pkt = 0;
	begin
		// local_obj BusPacket pkt(BusPacketType::write_data, source, address, value)
		// send_request(pkt)
	end
	endfunction
	
	
	function send_read_response(bus_packet_payload_t value, BusID source);
		reg[32:0] state = 0;
		memory_address_t addr = 0;
		BusPacket pkt = 0;
	begin
		addr = 0;
		// local_obj BusPacket pkt(BusPacketType::read_response, source, addr, value)
		// send_response(pkt)
	end
	endfunction
	
	
	function accept_request();
		reg[32:0] state = 0;
		BusPacket f = 0;
		return f = 0;
	begin
		// assert(request_busy)
		f = request_data;
		request_busy=0;
	end
	endfunction
	
	
	function get_response();
		reg[32:0] state = 0;
		BusPacket f = 0;
		return f = 0;
	begin
		// assert(response_busy)
		f = response_data;
		response_busy=0;
	end
	endfunction
	
	
	function send_request(BusPacket pkt);
		reg[32:0] state = 0;
	begin
		// assert(!(request_busy))
		request_data=pkt;
		request_busy=1;
	end
	endfunction
	
	
	function send_response(BusPacket pkt);
		reg[32:0] state = 0;
	begin
		// assert(!(response_busy))
		response_data=pkt;
		response_busy=1;
	end
	endfunction
endinterface;
