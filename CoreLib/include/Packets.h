#pragma once

#include "Defines.h"

namespace ecc
{

	struct FetchToDecodeBusPacket
	{
		execution_mask_t exec_mask;
		memory_address_t PC;
		instruction_t insn;
	};

	struct StoreToFetchPacket
	{
		execution_mask_t exec_mask;
		memory_address_t newpc;
	};

	enum class BusPacketType : uint8_t
	{
		bus_read_data,
		bus_write_data,
		bus_read_response,
		bus_write_response
	};

	using bus_packet_payload_t = uint64_t;

	struct BusPacket
	{
		BusPacketType packet_type;
		BusID source;
		memory_address_t address;
		bus_packet_payload_t payload;
	};


	METHOD_SECTION;

	static inline
	BusPacket create_bus_packet(BusPacketType pkt_type, BusID source, memory_address_t address, bus_packet_payload_t payload)
	{
		BusPacket pkt;
		pkt.packet_type = pkt_type;
		pkt.source = source;
		pkt.address = address;
		pkt.payload = payload;
		return pkt;
	}




	static inline
	FetchToDecodeBusPacket create_fetch_decode_packet(execution_mask_t exec_mask,
		memory_address_t PC,
		instruction_t insn)
	{
		FetchToDecodeBusPacket pkt;
		pkt.exec_mask = exec_mask;
		pkt.insn = insn;
		pkt.PC = PC;
		return pkt;		
	}
}