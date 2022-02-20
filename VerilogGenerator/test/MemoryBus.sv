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
	begin
		request_data.packet_type <= bus_read_data;
		request_data.source <= source;
		request_data.address <= address;
		request_data.payload <= 0;
		request_busy <= 1;
	end
	end
	endfunction
	
	
	function void send_write_request_data(input memory_address_t address, input BusID source, input bus_packet_payload_t payload_data);
	begin
	begin
		request_data.packet_type <= bus_write_data;
		request_data.source <= source;
		request_data.address <= address;
		request_data.payload <= payload_data;
		request_busy <= 1;
	end
	end
	endfunction
	
	
	function void send_read_response(input bus_packet_payload_t value, input BusID source);
	begin
	begin
		response_data.packet_type <= bus_read_response;
		response_data.source <= source;
		response_data.address <= 0;
		response_data.payload <= value;
		response_busy <= 1;
	end
	end
	endfunction
	
	
	function BusPacket accept_request();
	begin
	begin
		assert(request_busy);
		request_busy <= 0;
		return request_data;
	end
	end
	endfunction
	
	
	function BusPacket get_response();
	begin
	begin
		assert(response_busy);
		response_busy <= 0;
		$display("response data = %x !!!!!!!!!!!!!!", response_data.payload);
		return response_data;
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
