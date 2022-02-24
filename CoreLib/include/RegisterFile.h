#pragma once

#include <cassert>

#include "MachineInfo.h"
#include "VectorValue.h"

namespace ecc
{

	INTERFACE RegisterFile
	{
		using TinyCounter = uint8_t;

		std::array<VectorValue, static_cast<int>(MAX_REG_ID)> regs;

		// incremented if reg[i] has been invalidated
		std::array<TinyCounter, static_cast<int>(MAX_REG_ID)> invalidated_regs;

		// incremented if reg[i] has been written to by the store-stage
		std::array<TinyCounter, static_cast<int>(MAX_REG_ID)> written_regs;

		flags_reg_t machine_flags = 0;

		METHOD_SECTION;

		void mark_invalid(const RegisterID id)
		{
			assert(isValidIndex(id));
			invalidated_regs[static_cast<int>(id)] += 1;
		}

		void mark_valid(const RegisterID id)
		{
			assert(isValidIndex(id));
			written_regs[static_cast<int>(id)] += 1;
		}

		bool is_valid(const RegisterID id)
		{
			assert(isValidIndex(id));
			return written_regs[static_cast<int>(id)] == invalidated_regs[static_cast<int>(id)];
		}

		void set(const RegisterID id, const VectorValue& v)
		{
			assert(isValidIndex(id));
			regs[static_cast<int>(id)] = v;
		}

		VectorValue get(const RegisterID id) const
		{
			assert(isValidIndex(id));
			return regs[static_cast<int>(id)];
		}

		void setMachineFlag(uint64_t flags)
		{
			machine_flags |= flags;
		}

		void setHasHalted()
		{
			machine_flags |= MACHINE_FLAGS_MASK_HALT;
		}


		bool hasHalted() const
		{
			return (machine_flags & MACHINE_FLAGS_MASK_HALT) != 0;
		}
	};
}