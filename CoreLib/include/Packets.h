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
		read_data,
		write_data,
		read_response,
		write_response
	};

	using bus_packet_payload_t = uint64_t;

	struct BusPacket
	{
		BusPacketType packet_type;
		BusID source;
		memory_address_t address;
		bus_packet_payload_t payload;
	};
}