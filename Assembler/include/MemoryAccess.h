#pragma once

#include "MachineInfo.h"

struct MemoryAccess
{
	MachineInfo::RegisterID base_reg;
	int32_t offset;

	MemoryAccess(MachineInfo::RegisterID reg, int32_t off)
	{
		base_reg = reg;
		offset = off;
	}
};