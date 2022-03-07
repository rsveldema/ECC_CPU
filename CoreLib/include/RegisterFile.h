#pragma once

#include <cassert>

#include "MachineInfo.h"
#include "VectorValue.h"

namespace ecc
{
	using TinyCounter = uint8_t;

	struct RegisterSet
	{
		// incremented if reg[i] has been invalidated
		std::array<TinyCounter, static_cast<int>(MAX_REG_ID)> invalidated_regs;

		// incremented if reg[i] has been written to by the store-stage
		std::array<TinyCounter, static_cast<int>(MAX_REG_ID)> written_regs;
		std::array<VectorValue, static_cast<int>(MAX_REG_ID)> regs;
		flags_reg_t machine_flags;
	};


	INTERFACE RegisterFile
	{
		RegisterSet data;

		METHOD_SECTION;

		void init()
		{
			data.machine_flags = 0;
		}

		void mark_invalid(const RegisterID id)
		{
			assert(isValidIndex(id));
			data.invalidated_regs[static_cast<int>(id)] += 1;
		}

		void mark_valid(const RegisterID id)
		{
			assert(isValidIndex(id));
			data.written_regs[static_cast<int>(id)] += 1;
		}

		bool is_valid(const RegisterID id)
		{
			assert(isValidIndex(id));
			return data.written_regs[static_cast<int>(id)] == data.invalidated_regs[static_cast<int>(id)];
		}

		void set(const RegisterID id, const VectorValue& v)
		{
			assert(isValidIndex(id));
			data.regs[static_cast<int>(id)] = v;
		}

		VectorValue get(const RegisterID id) const
		{
			assert(isValidIndex(id));
			return data.regs[static_cast<int>(id)];
		}

		void setMachineFlag(uint64_t flags)
		{
			data.machine_flags |= flags;
		}

		void setHasHalted()
		{
			data.machine_flags |= MACHINE_FLAGS_MASK_HALT;
			$finish();
		}


		bool hasHalted() const
		{
			return (data.machine_flags & MACHINE_FLAGS_MASK_HALT) != 0;
		}
	};
}