#pragma once

#include <cassert>

#include "MachineInfo.h"
#include "VectorValue.h"
#include "ExecutionMask.h"

namespace Simulator
{
	static bool isValid(const MachineInfo::RegisterID id)
	{
		return
			(id >= MachineInfo::RegisterID::R0) &&
			(id < MachineInfo::RegisterID::MAX_REG_ID);
	}

	class RegisterFile
	{
	public:
		ExecutionMask exec_mask;

		VectorValue regs[static_cast<int>(MachineInfo::RegisterID::MAX_REG_ID)];
		uint32_t machine_flags = 0;

		VectorValue& operator [](const MachineInfo::RegisterID id)
		{
			assert(isValid(id));
			return regs[static_cast<int>(id)];
		}

		VectorValue operator [](const MachineInfo::RegisterID id) const
		{
			assert(isValid(id));
			return regs[static_cast<int>(id)];
		}

		void setMachineFlag(uint32_t flags)
		{
			machine_flags |= flags;
		}

		void setHasHalted()
		{
			machine_flags |= MachineInfo::MACHINE_FLAGS_MASK_HALT;
		}


		bool hasHalted() const
		{
			return machine_flags & MachineInfo::MACHINE_FLAGS_MASK_HALT;
		}
	};
}