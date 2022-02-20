#pragma once

#include <variant>
#include <array>

#include "MachineInfo.h"

namespace ecc
{
	struct VectorValue
	{
		using elt_t = int64_t;

		elt_t data[NUMBER_OF_VECTOR_THREADS_INT64];

		uint32_t size() const
		{
			return NUMBER_OF_VECTOR_THREADS_INT64;
		}


		elt_t get(unsigned ix) const
		{
			assert(ix < size());
			return data[ix];
		}

		void set(unsigned ix, int64_t value)
		{
			assert(ix < size());
			data[ix] = value;
		}

		
		memory_address_t get_PC() const
		{
			return data[0];
		}

		void setPC(memory_address_t value)
		{
			data[0] = {static_cast<elt_t>(value)};
		}
	};

	static VectorValue or_shift_left(const VectorValue &self, const VectorValue &other, unsigned shift_count)
	{
		VectorValue ret;
		for (uint32_t i = 0; i < self.size(); i++)
		{
			ret.data[i] = self.data[i] | (other.data[i] << shift_count);
		}
		return ret;
	}

	static VectorValue shift_left(const VectorValue &self, const VectorValue &other)
	{
		VectorValue ret;
		for (uint32_t i = 0; i < self.size(); i++)
		{
			ret.data[i] = self.data[i] << other.data[i];
		}
		return ret;
	}

	static VectorValue add(const VectorValue &self, const VectorValue &other)
	{
		VectorValue ret;
		for (uint32_t i = 0; i < self.size(); i++)
		{
			ret.data[i] = self.data[i] + other.data[i];
		}
		return ret;
	}

	static VectorValue bit_and(const VectorValue &self, const VectorValue &other)
	{
		VectorValue ret;
		for (uint32_t i = 0; i < self.size(); i++)
		{
			ret.data[i] = self.data[i] & other.data[i];
		}
		return ret;
	}

	static bool all_equal(const VectorValue &self)
	{
		for (uint32_t i = 1; i < self.size(); i++)
		{
			if (self.data[0] != self.data[i])
			{
				return false;
			}
		}
		return true;
	}

	static VectorValue compare_vecs(const VectorValue &self, const VectorValue &other)
	{
		VectorValue ret;
		for (uint32_t i = 0; i < self.size(); i++)
		{
			int64_t result = 0;

			const auto value1 = self.data[i];
			const auto value2 = other.data[i];

			if (value1 == value2)
				result |= FLAGS_MASK_EQ;

			if (value1 > value2)
				result |= FLAGS_MASK_GT;

			if (value1 < value2)
				result |= FLAGS_MASK_LT;

			ret.data[i] = result;
		}
		return ret;
	}

	static bool are_all_adjacent_memory_addresses(const VectorValue &v, int64_t elt_size)
	{
		int64_t first = v.data[0];
		for (uint32_t i = 1; i < v.size(); i++)
		{
			if (v.data[i] != (v.data[i - 1] + elt_size))
			{
				return false;
			}
		}
		return true;
	}

	static bool areAllValidMemoryAddresses(const VectorValue &v)
	{
		return true;
	}

	static uint64_t reduce_to_uint64_t(const VectorValue &v)
	{
		uint64_t ret = 0;
		for (uint32_t i = 0; i < v.size(); i++)
		{
			uint64_t tst = (v.data[i] != 0);
			ret |= tst << i;
		}
		return ret;
	}

	static VectorValue create_vec_incrementing_values()
	{
		VectorValue ret;
		for (uint32_t i = 0; i < ret.size(); i++)
		{
			ret.data[i] = i;
		}
		return ret;
	}

	static VectorValue create_vec_int64(const int64_t v)
	{
		VectorValue ret;
		for (uint32_t i = 0; i < ret.size(); i++)
		{
			ret.data[i] = v;
		}
		return ret;
	}

	static std::string to_string(const VectorValue &v)
	{
		std::string s = "<";
		const char *comma = "";
		for (auto &v : v.data)
		{
			s += comma;
			s += std::to_string(v);
			comma = ", ";
		}

		s += ">";
		return s;
	}
}