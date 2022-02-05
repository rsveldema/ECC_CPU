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
	
}