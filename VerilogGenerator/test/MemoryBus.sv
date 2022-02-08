interface MemoryBus;
	bool request_busy;
	bool response_busy;
	BusPacket request_data;
	BusPacket response_data;
	
	
	function void init_memory_bus();
	begin
		request_busy=0;
		response_busy=0;
	end
	endfunction
	
	
	function void send_read_request_data(memory_address_t address, BusID source);
		BusPacket pkt;
	begin
		pkt = create_bus_packet(BusPacketType::bus_read_data, source, address, 0);
		send_request(pkt);
	end
	endfunction
	
	
	function void send_write_request_data(memory_address_t address, BusID source, bus_packet_payload_t value);
		BusPacket pkt;
	begin
		pkt = create_bus_packet(BusPacketType::bus_write_data, source, address, value);
		send_request(pkt);
	end
	endfunction
	
	
	function void send_read_response(bus_packet_payload_t value, BusID source);
		memory_address_t addr;
		BusPacket pkt;
	begin
		addr = 0;
		pkt = create_bus_packet(BusPacketType::bus_read_response, source, addr, value);
		send_response(pkt);
	end
	endfunction
	
	
	function BusPacket accept_request();
		BusPacket f;
	begin
		assert(request_busy);
		f = request_data;
		request_busy=0;
		return f;
	end
	endfunction
	
	
	function BusPacket get_response();
		BusPacket f;
	begin
		assert(response_busy);
		f = response_data;
		response_busy=0;
		return f;
	end
	endfunction
	
	
	function void send_request(BusPacket pkt);
	begin
		assert(!(request_busy));
		request_data=pkt;
		request_busy=1;
	end
	endfunction
	
	
	function void send_response(BusPacket pkt);
	begin
		assert(!(response_busy));
		response_data=pkt;
		response_busy=1;
	end
	endfunction
endinterface;
