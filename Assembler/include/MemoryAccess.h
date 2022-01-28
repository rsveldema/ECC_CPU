#pragma once

#include "MachineInfo.h"

struct MemoryAccess
{
	ecc::RegisterID base_reg;
	int32_t offset;

	MemoryAccess(ecc::RegisterID reg, int32_t off)
	{
		base_reg = reg;
		offset = off;
	}
};