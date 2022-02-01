#pragma once

#include <cassert>

#include "MachineInfo.h"
#include "VectorValue.h"

namespace ecc
{
	static bool isValidIndex(const ecc::RegisterID id)
	{
		return
			(id >= ecc::RegisterID::REG_R0) &&
			(id < ecc::RegisterID::MAX_REG_ID);
	}

	class RegisterFile
	{
	public:

		using TinyCounter = uint8_t;

		std::array<VectorValue, static_cast<int>(ecc::RegisterID::MAX_REG_ID)> regs;

		// incremented if reg[i] has been invalidated
		std::array<TinyCounter, static_cast<int>(ecc::RegisterID::MAX_REG_ID)> invalidated_regs;

		// incremented if reg[i] has been written to by the store-stage
		std::array<TinyCounter, static_cast<int>(ecc::RegisterID::MAX_REG_ID)> written_regs;

		uint32_t machine_flags = 0;

		void mark_invalid(const ecc::RegisterID id)
		{
			assert(isValidIndex(id));
			invalidated_regs[static_cast<int>(id)]++;
		}

		void mark_valid(const ecc::RegisterID id)
		{
			assert(isValidIndex(id));
			written_regs[static_cast<int>(id)]++;
		}

		bool is_valid(const ecc::RegisterID id)
		{
			assert(isValidIndex(id));
			return written_regs[static_cast<int>(id)] == invalidated_regs[static_cast<int>(id)];
		}

		VectorValue& operator [](const ecc::RegisterID id)
		{
			assert(isValidIndex(id));
			return regs[static_cast<int>(id)];
		}

		VectorValue operator [](const ecc::RegisterID id) const
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
			machine_flags |= ecc::MACHINE_FLAGS_MASK_HALT;
		}


		bool hasHalted() const
		{
			return machine_flags & ecc::MACHINE_FLAGS_MASK_HALT;
		}
	};
}