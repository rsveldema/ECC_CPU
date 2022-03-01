

typedef enum byte {
	VEC_BUS_PKT_TYPE_read_vec64
	,VEC_BUS_PKT_TYPE_write_vec64
	,VEC_BUS_PKT_TYPE_read_response_vec64
	,VEC_BUS_PKT_TYPE_write_response_vec64
} VecBusPacketType;

typedef VectorValue vec_bus_payload_t;


typedef struct packed {
	VecBusPacketType packet_type;
	BusID source;
	VectorValue address;
	vec_bus_payload_t payload;
} VecBusPacket;
interface VecMemoryBus;
	bool request_busy;
	VecBusPacket request_queue;
	bool response_busy;
	VecBusPacket response_queue;
	
	
	function void init();
	begin
	begin
		request_busy = 0;
		response_busy = 0;
	end
	end
	endfunction
	
	
	function void send_read_request_vec(input VectorValue address, input BusID source);
	begin
	begin
		assert(!(request_busy));
		request_queue.source <= source;
		request_queue.address <= address;
		request_queue.packet_type <= VEC_BUS_PKT_TYPE_read_vec64;
		request_busy <= 1;
	end
	end
	endfunction
	
	
	function void send_write_request_vec(input VectorValue address, input BusID source, input VectorValue value);
	begin
	begin
		request_queue.source <= source;
		request_queue.address <= address;
		request_queue.payload <= value;
		request_queue.packet_type <= VEC_BUS_PKT_TYPE_write_vec64;
		request_busy <= 1;
	end
	end
	endfunction
	
	
	function VecBusPacket accept_request();
	begin
	begin
		assert(request_busy);
		request_busy <= 0;
		return request_queue;
	end
	end
	endfunction
	
	
	function VecBusPacket accept_response();
	begin
	begin
		assert(response_busy);
		response_busy <= 0;
		return response_queue;
	end
	end
	endfunction
	
	
	function void send_response(input VecBusPacket pkt);
	begin
	begin
		assert(!(response_busy));
		response_queue <= pkt;
		response_busy <= 1;
	end
	end
	endfunction
endinterface;
