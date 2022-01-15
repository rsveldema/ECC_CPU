#pragma once

#include "MachineInfo.h"

struct MemoryAccess
{
	MachineInfo::Register base_reg;
	int32_t offset;

	MemoryAccess(MachineInfo::Register reg, int32_t off)
	{
		base_reg = reg;
		offset = off;
	}
};