#pragma once

#include <cassert>

namespace Simulator
{

	class RegisterFile
	{
	public:
		using Register = int64_t;

		Register regs[static_cast<int>(MachineInfo::RegisterID::MAX_REG_ID)];

		Register& operator [](const MachineInfo::RegisterID id)
		{
			assert(isValid(id));
			return regs[static_cast<int>(id)];
		}

		Register operator [](const MachineInfo::RegisterID id) const
		{
			assert(isValid(id));
			return regs[static_cast<int>(id)];
		}

		bool isValid(const MachineInfo::RegisterID id) const
		{
			return
				(id >= MachineInfo::RegisterID::R0) &&
				(id < MachineInfo::RegisterID::MAX_REG_ID);
		}

		bool hasHalted() const
		{
			return (*this)[MachineInfo::RegisterID::FLAGS] & MachineInfo::FLAGS_MASK_HALT;
		}
	};
}