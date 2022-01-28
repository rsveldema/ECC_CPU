#pragma once

#include <cassert>

#include "MachineInfo.h"
#include "VectorValue.h"

namespace Simulator
{
	static bool isValidIndex(const MachineInfo::RegisterID id)
	{
		return
			(id >= MachineInfo::RegisterID::R0) &&
			(id < MachineInfo::RegisterID::MAX_REG_ID);
	}

	class RegisterFile
	{
	public:

		using TinyCounter = uint8_t;

		std::array<VectorValue, static_cast<int>(MachineInfo::RegisterID::MAX_REG_ID)> regs;

		// incremented if reg[i] has been invalidated
		std::array<TinyCounter, static_cast<int>(MachineInfo::RegisterID::MAX_REG_ID)> invalidated_regs;

		// incremented if reg[i] has been written to by the store-stage
		std::array<TinyCounter, static_cast<int>(MachineInfo::RegisterID::MAX_REG_ID)> written_regs;

		uint32_t machine_flags = 0;

		void mark_invalid(const MachineInfo::RegisterID id)
		{
			assert(isValidIndex(id));
			invalidated_regs[static_cast<int>(id)]++;
		}

		void mark_valid(const MachineInfo::RegisterID id)
		{
			assert(isValidIndex(id));
			written_regs[static_cast<int>(id)]++;
		}

		bool is_valid(const MachineInfo::RegisterID id)
		{
			assert(isValidIndex(id));
			return written_regs[static_cast<int>(id)] == invalidated_regs[static_cast<int>(id)];
		}

		VectorValue& operator [](const MachineInfo::RegisterID id)
		{
			assert(isValidIndex(id));
			return regs[static_cast<int>(id)];
		}

		VectorValue operator [](const MachineInfo::RegisterID id) const
		{
			assert(isValidIndex(id));
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