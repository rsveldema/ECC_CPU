

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


function BusPacket create_bus_packet(BusPacketType pkt_type, BusID source, memory_address_t address, bus_packet_payload_t payload);
	BusPacket pkt;
begin
	pkt.packet_type=pkt_type;
	pkt.source=source;
	pkt.address=address;
	pkt.payload=payload;
	return pkt;
end
endfunction


function FetchToDecodeBusPacket create_fetch_decode_packet(execution_mask_t exec_mask, memory_address_t PC, instruction_t insn);
	FetchToDecodeBusPacket pkt;
begin
	pkt.exec_mask=exec_mask;
	pkt.insn=insn;
	pkt.PC=PC;
	return pkt;
end
endfunction
