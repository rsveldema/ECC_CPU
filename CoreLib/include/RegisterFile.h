#pragma once


namespace Simulator
{

	class RegisterFile
	{
	public:
		int64_t regs[static_cast<int>(MachineInfo::Register::MAX_REG_ID)];

		int64_t& operator [](const MachineInfo::Register& id)
		{
			return regs[static_cast<int>(id)];
		}

		int64_t operator [](const MachineInfo::Register& id) const
		{
			return regs[static_cast<int>(id)];
		}

		bool hasHalted() const
		{
			return (*this)[MachineInfo::Register::FLAGS] & MachineInfo::REG_HALT_FLAG;
		}
	};
}