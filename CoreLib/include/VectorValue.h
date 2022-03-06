#pragma once

#include <cassert>
#include <variant>
#include <array>

#include "MachineInfo.h"
#include "Defines.h"
#include "Task.h"


namespace ecc
{
	struct VectorValue
	{
		std::array<int64_t, NUMBER_OF_VECTOR_THREADS_INT64> data;
	};

	METHOD_SECTION;


	static inline int64_t get(const VectorValue &v, uint32_t ix)
	{
		assert(ix < NUMBER_OF_VECTOR_THREADS_INT64);
		return v.data[ix];
	}

	/*
	void set(uint32_t ix, int64_t value)
	{
		assert(ix < NUMBER_OF_VECTOR_THREADS_INT64);
		data[ix] = value;
	}*/

	static VectorValue or_shift_left(const VectorValue &self, const VectorValue &other, uint32_t shift_count)
	{
		return {
			(self.data[0] | (other.data[0] << shift_count)),
			(self.data[1] | (other.data[1] << shift_count)),
			(self.data[2] | (other.data[2] << shift_count)),
			(self.data[3] | (other.data[3] << shift_count)),
			(self.data[4] | (other.data[4] << shift_count)),
			(self.data[5] | (other.data[5] << shift_count)),
			(self.data[6] | (other.data[6] << shift_count)),
			(self.data[7] | (other.data[7] << shift_count))
		};
	}

	static VectorValue shift_left(const VectorValue &self, const VectorValue &other)
	{
		return {
			(self.data[0] << other.data[0]),
			(self.data[1] << other.data[1]),
			(self.data[2] << other.data[2]),
			(self.data[3] << other.data[3]),
			(self.data[4] << other.data[4]),
			(self.data[5] << other.data[5]),
			(self.data[6] << other.data[6]),
			(self.data[7] << other.data[7])
		};
	}

	static VectorValue add(const VectorValue &self, const VectorValue &other)
	{
		return {
			(self.data[0] + other.data[0]),
			(self.data[1] + other.data[1]),
			(self.data[2] + other.data[2]),
			(self.data[3] + other.data[3]),
			(self.data[4] + other.data[4]),
			(self.data[5] + other.data[5]),
			(self.data[6] + other.data[6]),
			(self.data[7] + other.data[7])
		};
	}

	static VectorValue bit_and(const VectorValue &self, const VectorValue &other)
	{
		return {
			(self.data[0] & other.data[0]),
			(self.data[1] & other.data[1]),
			(self.data[2] & other.data[2]),
			(self.data[3] & other.data[3]),
			(self.data[4] & other.data[4]),
			(self.data[5] & other.data[5]),
			(self.data[6] & other.data[6]),
			(self.data[7] & other.data[7])
		};
	}

	static bool all_equal(const VectorValue &self)
	{
		for (uint32_t i = 1; i < NUMBER_OF_VECTOR_THREADS_INT64; i++)
		{
			if (self.data[0] != self.data[i])
			{
				return false;
			}
		}
		return true;
	}

	static int64_t get_compare_result(int64_t value1, int64_t value2)
	{
		if (value1 == value2)
		{
			return FLAGS_MASK_EQ;
		}

		if (value1 > value2)
		{
			return FLAGS_MASK_GT;
		}

		// if (value1 < value2) {
		return FLAGS_MASK_LT;
		//}
	}

	static VectorValue compare_vecs(const VectorValue &self, const VectorValue &other)
	{
		return {
				get_compare_result(self.data[0], other.data[0]),
				get_compare_result(self.data[1], other.data[1]),
				get_compare_result(self.data[2], other.data[2]),
				get_compare_result(self.data[3], other.data[3]),
				get_compare_result(self.data[4], other.data[4]),
				get_compare_result(self.data[5], other.data[5]),
				get_compare_result(self.data[6], other.data[6]),
				get_compare_result(self.data[7], other.data[7])
		};
	}

	static bool are_all_adjacent_memory_addresses(const VectorValue &v, int64_t elt_size)
	{
		for (uint32_t i = 1; i < NUMBER_OF_VECTOR_THREADS_INT64; i++)
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
		return PACK8_BITS(  
				(v.data[0] != 0), 
				(v.data[1] != 0), 
				(v.data[2] != 0), 
				(v.data[3] != 0), 
				(v.data[4] != 0), 
				(v.data[5] != 0), 
				(v.data[6] != 0), 
				(v.data[7] != 0));
	}

	static VectorValue create_vec_incrementing_values()
	{
		return {
			static_cast<int64_t>(0),
			static_cast<int64_t>(1),
			static_cast<int64_t>(2),
			static_cast<int64_t>(3),
			static_cast<int64_t>(4),
			static_cast<int64_t>(5),
			static_cast<int64_t>(6),
			static_cast<int64_t>(7)
		};
	}

	static VectorValue create_vec_int64(const int64_t v)
	{
		return {v, v, v, v, v, v, v, v};
	}

	static VectorValue create_vec_int8(const int8_t v)
	{
		return create_vec_int64(static_cast<int64_t>(v));
	}

	static VectorValue create_vec_int16(const int16_t v)
	{
		return create_vec_int64(static_cast<int64_t>(v));
	}

	static VectorValue create_vec_int32(const int32_t v)
	{
		return create_vec_int64(static_cast<int64_t>(v));
	}
}
