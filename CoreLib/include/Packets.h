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

	enum BusPacketType : uint8_t
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
	
	union DecodeStageValue
	{
		RegisterID regID;
		VectorValue vec;
	};

	struct DecodeExecPacket
	{
		execution_mask_t exec_mask;

		memory_address_t PC;
		ExecuteStageOpcode opcode;

		DecodeStageValue value0;
		VectorValue value1;
		VectorValue value2;
	};


	METHOD_SECTION;

	static inline
	BusPacket create_bus_packet(BusPacketType pkt_type, BusID source, memory_address_t address, bus_packet_payload_t payload)
	{
		return { pkt_type, source, address, payload };
	}

	static inline
	FetchToDecodeBusPacket create_fetch_decode_packet(execution_mask_t exec_mask,
		memory_address_t PC,
		instruction_t insn)
	{
		return { exec_mask, PC, insn };
	}
}