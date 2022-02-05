#pragma once

#include <variant>
#include <array>

#include "MachineInfo.h"


namespace ecc
{
	template <typename ElementType>
	struct vec_vector_obj_t
	{
		
	};


	struct VectorValue
	{
		using elt_t = int64_t;

		using elt_vector_t = std::array<elt_t, NUMBER_OF_VECTOR_THREADS_INT64>;

		elt_vector_t data;

		std::string to_string() const
		{
			std::string s = "<";
			const char* comma = "";
			for (auto& v : data)
			{
				s += comma;
				s += std::to_string(v);
				comma = ", ";
			}

			s += ">";
			return s;
		}


		bool areAllValidMemoryAddresses() const
		{
			return true;
		}

		void replicate(const elt_t v)
		{
			for (int i = 0; i < data.size(); i++)
			{
				data[i] = v;
			}
		}

		void set_incrementing_values()
		{
			for (int i = 0; i < data.size(); i++)
			{
				data[i] = i;
			}
		}

		void or_shift_left(const VectorValue& other, unsigned shift_count)
		{
			for (int i = 0; i < data.size(); i++)
			{
				data[i] |= other.data[i] << shift_count;
			}
		}

		void shift_left(const VectorValue& other)
		{
			for (int i = 0; i < data.size(); i++)
			{
				data[i] <<= other.data[i];
			}
		}

		void add(const VectorValue& other)
		{
			for (int i = 0; i < data.size(); i++)
			{
				data[i] += other.data[i];
			}
		}

		void bit_and(const VectorValue& other)
		{
			for (int i = 0; i < data.size(); i++)
			{
				data[i] &= other.data[i];
			}
		}


		bool all_equal() const
		{
			for (int i = 1; i < data.size(); i++)
			{
				if (data[0] != data[i])
				{
					return false;
				}
			}
			return true;
		}

		VectorValue compare(const VectorValue& other) const
		{
			VectorValue ret;
			for (int i = 0; i < data.size(); i++)
			{
				elt_t result = 0;

				const auto value1 = data[i];
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

		void store_at(uint8_t* ptr) const
		{
			elt_t* out = reinterpret_cast<elt_t*>(ptr);

			std::copy(data.begin(), data.end(), out);
		}

		/*
		void load_from(uint8_t* ptr)
		{
			elt_t* out = reinterpret_cast<elt_t*>(data.data());

			std::copy(out, out + data.size(), data.begin());
		}
		*/

		elt_t get(unsigned ix) const
		{
			assert(ix < data.size());
			return data[ix];
		}

		bool are_all_adjacent_memory_addresses(int64_t elt_size) const
		{
			elt_t first = data[0];
			for (int i = 1; i < data.size(); i++)
			{
				if (data[i] != (data[i - 1] + elt_size))
				{
					return false;
				}
			}
			return true;
		}

		uint64_t reduce_to_uint64_t() const
		{
			uint64_t ret = 0;
			for (unsigned i = 0; i < data.size(); i++)
			{
				uint64_t v = (data[i] != 0);
				ret |= v << i;
			}
			return ret;
		}

		size_t size() const
		{
			return data.size();
		}
		

		void set(unsigned ix, int8_t value)
		{
			assert(ix < data.size());
			data[ix] = value;
		}

		void set(unsigned ix, int16_t value)
		{
			assert(ix < data.size());
			data[ix] = value;
		}

		void set(unsigned ix, int32_t value)
		{
			assert(ix < data.size());
			data[ix] = value;
		}

		void set(unsigned ix, int64_t value)
		{
			assert(ix < data.size());
			data[ix] = value;
		}

		void set(unsigned ix, float value)
		{
			assert(ix < data.size());
			elt_t tmp = *(int32_t*) &value;
			data[ix] = tmp;
		}

		void set(unsigned ix, double value)
		{
			assert(ix < data.size());
			elt_t tmp = *(int64_t*) &value;
			data[ix] = tmp;
		}

		uint64_t reduce_int64_to_single_int64_t() const
		{
			return reduce_to_uint64_t();
		}

		/*
		void load_from_int64(uint8_t* ptr)
		{
			load_from(ptr);
		}
		*/


		memory_address_t get_PC() const
		{
			return get_int64(0);
		}

		void setPC(memory_address_t value)
		{
			data = { static_cast<elt_t>(value) };
		}

		int64_t get_int64(unsigned ix) const
		{
			assert(ix < data.size());
			return get(ix);
		}

		void set_int64(unsigned ix, int64_t value)
		{
			assert(ix < data.size());
			data[ix] = value;
		}

		static VectorValue create_vec_int64_blockindex()
		{
			VectorValue ret;
			ret.set_incrementing_values();
			return ret;
		}

		static VectorValue create_vec_int64(int64_t v)
		{
			VectorValue ret;
			ret.replicate(v);
			return ret;
		}

		VectorValue shift_left_int64(const VectorValue& v) const
		{
			VectorValue ret(*this);
			ret.shift_left(v);
			return ret;
		}

		VectorValue or_shift_left_int64(const VectorValue& v, int shift_count) const
		{
			VectorValue ret(*this);
			ret.or_shift_left(v, shift_count);
			return ret;
		}


		VectorValue add_int64(const VectorValue& v) const
		{
			VectorValue ret(*this);
			ret.add(v);
			return ret;
		}


		VectorValue bit_and_int64(const VectorValue& v) const
		{
			VectorValue ret(*this);
			ret.bit_and(v);
			return ret;
		}

		VectorValue compare_int64(const VectorValue& v) const
		{
			VectorValue ret = compare(v);
			return ret;
		}

		bool all_equal_int64() const
		{
			return all_equal();
		}
	};


	static std::string to_string(const VectorValue& v)
	{
		return v.to_string();
	}

}