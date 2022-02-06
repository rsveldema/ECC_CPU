

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
	read_data
	,write_data
	,read_response
	,write_response
} BusPacketType;

typedef uint64_t bus_packet_payload_t;


typedef struct packed {
	BusPacketType packet_type;
	BusID source;
	memory_address_t address;
	bus_packet_payload_t payload;
} BusPacket;
