

typedef struct packed {
	execution_mask_t exec_mask;
	memory_address_t PC;
	instruction_t insn;
} FetchToDecodeBusPacket;


typedef struct packed {
	execution_mask_t exec_mask;
	memory_address_t newpc;
} StoreToFetchPacket;


typedef enum {
	bus_read_data
	,bus_write_data
	,bus_read_response
	,bus_write_response
} BusPacketType;

typedef uint64_t bus_packet_payload_t;


typedef struct packed {
	BusPacketType packet_type;
	BusID source;
	memory_address_t address;
	bus_packet_payload_t payload;
} BusPacket;


typedef union packed {
	RegisterID regID;
	VectorValue vec;
} DecodeStageValue;


typedef struct packed {
	execution_mask_t exec_mask;
	memory_address_t PC;
	ExecuteStageOpcode opcode;
	DecodeStageValue value0;
	VectorValue value1;
	VectorValue value2;
} DecodeExecPacket;


function BusPacket create_bus_packet(input BusPacketType pkt_type, input BusID source, input memory_address_t address, input bus_packet_payload_t payload);
begin
begin
	return {pkt_type, source, address, payload};
end
end
endfunction


function FetchToDecodeBusPacket create_fetch_decode_packet(input execution_mask_t exec_mask, input memory_address_t PC, input instruction_t insn);
begin
begin
	return {exec_mask, PC, insn};
end
end
endfunction
