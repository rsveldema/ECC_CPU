interface MemoryBus;
	bool request_busy;
	bool response_busy;
	BusPacket request_data;
	BusPacket response_data;
	
	
	function void init();
	begin
	begin
		request_busy = 0;
		response_busy = 0;
	end
	end
	endfunction
	
	
	function void send_read_request_data(input memory_address_t address, input BusID source);
	begin
		BusPacket pkt;
	begin
		pkt <= create_bus_packet(bus_read_data, source, address, 0);
		send_request(pkt);
	end
	end
	endfunction
	
	
	function void send_write_request_data(input memory_address_t address, input BusID source, input bus_packet_payload_t payload_data);
	begin
		BusPacket pkt;
	begin
		pkt <= create_bus_packet(bus_write_data, source, address, payload_data);
		send_request(pkt);
	end
	end
	endfunction
	
	
	function void send_read_response(input bus_packet_payload_t value, input BusID source);
	begin
		memory_address_t addr;
		BusPacket pkt;
	begin
		addr <= 0;
		pkt <= create_bus_packet(bus_read_response, source, addr, value);
		send_response(pkt);
	end
	end
	endfunction
	
	
	function BusPacket accept_request();
	begin
		BusPacket f;
	begin
		assert(request_busy);
		f <= request_data;
		request_busy <= 0;
		return f;
	end
	end
	endfunction
	
	
	function BusPacket get_response();
	begin
		BusPacket f;
	begin
		assert(response_busy);
		f <= response_data;
		response_busy <= 0;
		return f;
	end
	end
	endfunction
	
	
	function void send_request(input BusPacket pkt);
	begin
	begin
		assert(!(request_busy));
		request_data <= pkt;
		request_busy <= 1;
	end
	end
	endfunction
	
	
	function void send_response(input BusPacket pkt);
	begin
	begin
		assert(!(response_busy));
		response_data <= pkt;
		response_busy <= 1;
	end
	end
	endfunction
endinterface;
