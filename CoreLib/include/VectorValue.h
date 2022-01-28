#pragma once

#include <variant>
#include <array>

#include "MachineInfo.h"


namespace ecc
{
	template <typename ElementType>
	struct vec_vector_obj_t
	{
		using elt_t = ElementType;
		using elt_vector_t = std::array<ElementType, ecc::VECTOR_MEM_SIZE / sizeof(ElementType)>;

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

		void replicate(const ElementType v)
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

		void or_shift_left(const vec_vector_obj_t& other, unsigned shift_count)
		{
			for (int i = 0; i < data.size(); i++)
			{
				data[i] |= other.data[i] << shift_count;
			}
		}

		void shift_left(const vec_vector_obj_t& other)
		{
			for (int i = 0; i < data.size(); i++)
			{
				data[i] <<= other.data[i];
			}
		}

		void add(const vec_vector_obj_t& other)
		{
			for (int i = 0; i < data.size(); i++)
			{
				data[i] += other.data[i];
			}
		}

		void bit_and(const vec_vector_obj_t& other)
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

		template<typename ret_vector_type>
		void compare(ret_vector_type& ret, const vec_vector_obj_t& other) const
		{
			for (int i = 0; i < data.size(); i++)
			{
				typename ret_vector_type::elt_t result = 0;

				const auto value1 = data[i];
				const auto value2 = other.data[i];

				if (value1 == value2)
					result |= ecc::FLAGS_MASK_EQ;

				if (value1 > value2)
					result |= ecc::FLAGS_MASK_GT;

				if (value1 < value2)
					result |= ecc::FLAGS_MASK_LT;

				ret.data[i] = result;
			}
		}

		void store_at(uint8_t* ptr) const
		{
			elt_t* out = reinterpret_cast<elt_t*>(ptr);

			std::copy(data.begin(), data.end(), out);
		}

		void load_from(uint8_t* ptr)
		{
			elt_t* out = reinterpret_cast<elt_t*>(data.data());

			std::copy(out, out + data.size(), data.begin());
		}

		void set(unsigned ix, ElementType value)
		{
			assert(ix < data.size());
			data[ix] = value;
		}

		ElementType get(unsigned ix) const
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
	};


	struct VectorValue
	{
		std::variant<vec_vector_obj_t<int8_t>,
			vec_vector_obj_t<int16_t>,
			vec_vector_obj_t<int32_t>,
			vec_vector_obj_t<int64_t>,

			vec_vector_obj_t<float>,
			vec_vector_obj_t<double>> data;

		enum class Type
		{
			INT8,
			INT16,
			INT32,
			INT64,
			FLT,
			DBL
		};

		std::string to_string() const
		{
			std::string s;
			s += "<";
			switch (getType())
			{
			case Type::INT8: s += get_int8_array().to_string(); break;
			case Type::INT16: s += get_int16_array().to_string(); break;
			case Type::INT32: s += get_int32_array().to_string(); break;
			case Type::INT64: s += get_int64_array().to_string(); break;
			case Type::FLT: s += get_float_array().to_string(); break;
			case Type::DBL: s += get_double_array().to_string(); break;
				break;
			}
			s += ">";
			return s;
		}

		void set(unsigned ix, int8_t value)
		{
			assert(getType() == Type::INT8);
			get_int8_array().set(ix, value);
		}

		void set(unsigned ix, int16_t value)
		{
			assert(getType() == Type::INT16);
			get_int16_array().set(ix, value);
		}

		void set(unsigned ix, int32_t value)
		{
			assert(getType() == Type::INT32);
			get_int32_array().set(ix, value);
		}

		void set(unsigned ix, int64_t value)
		{
			assert(getType() == Type::INT64);
			get_int64_array().set(ix, value);
		}

		void set(unsigned ix, float value)
		{
			assert(getType() == Type::FLT);
			get_float_array().set(ix, value);
		}

		void set(unsigned ix, double value)
		{
			assert(getType() == Type::DBL);
			get_double_array().set(ix, value);
		}


		Type getType() const
		{
			return static_cast<Type>(data.index());
		}

		vec_vector_obj_t<int8_t>& get_int8_array()
		{
			assert(getType() == Type::INT8);
			auto& vec = std::get< vec_vector_obj_t<int8_t> >(data);
			return vec;
		}

		const vec_vector_obj_t<int8_t>& get_int8_array() const
		{
			assert(getType() == Type::INT8);
			const auto& vec = std::get< vec_vector_obj_t<int8_t> >(data);
			return vec;
		}

		vec_vector_obj_t<int16_t>& get_int16_array()
		{
			assert(getType() == Type::INT16);
			auto& vec = std::get< vec_vector_obj_t<int16_t> >(data);
			return vec;
		}

		const vec_vector_obj_t<int16_t>& get_int16_array() const
		{
			assert(getType() == Type::INT16);
			const auto& vec = std::get< vec_vector_obj_t<int16_t> >(data);
			return vec;
		}

		vec_vector_obj_t<int32_t>& get_int32_array()
		{
			assert(getType() == Type::INT32);
			auto& vec = std::get< vec_vector_obj_t<int32_t> >(data);
			return vec;
		}

		const vec_vector_obj_t<int32_t>& get_int32_array() const
		{
			assert(getType() == Type::INT32);
			const auto& vec = std::get< vec_vector_obj_t<int32_t> >(data);
			return vec;
		}

		vec_vector_obj_t<int64_t>& get_int64_array()
		{
			assert(getType() == Type::INT64);
			auto& vec = std::get< vec_vector_obj_t<int64_t> >(data);
			return vec;
		}
		const vec_vector_obj_t<int64_t>& get_int64_array() const
		{
			assert(getType() == Type::INT64);
			const auto& vec = std::get< vec_vector_obj_t<int64_t> >(data);
			return vec;
		}

		vec_vector_obj_t<float>& get_float_array()
		{
			assert(getType() == Type::INT64);
			auto& vec = std::get< vec_vector_obj_t<float> >(data);
			return vec;
		}

		const vec_vector_obj_t<float>& get_float_array() const
		{
			assert(getType() == Type::INT64);
			const auto& vec = std::get< vec_vector_obj_t<float> >(data);
			return vec;
		}

		vec_vector_obj_t<double>& get_double_array()
		{
			assert(getType() == Type::INT64);
			auto& vec = std::get< vec_vector_obj_t<double> >(data);
			return vec;
		}

		const vec_vector_obj_t<double>& get_double_array() const
		{
			assert(getType() == Type::INT64);
			auto& vec = std::get< vec_vector_obj_t<double> >(data);
			return vec;
		}


		uint64_t reduce_int64_to_single_int64_t() const
		{
			assert(getType() == Type::INT64);
			auto& vec = std::get< vec_vector_obj_t<int64_t> >(data);
			return vec.reduce_to_uint64_t();
		}

		void load_from_int64(uint8_t* ptr)
		{
			data = vec_vector_obj_t<int64_t>{ { } };
			auto& vec = get_int64_array();
			vec.load_from(ptr);
		}

		void store_at(uint8_t* ptr) const
		{
			switch (data.index())
			{
			case 0: // int8
			{ abort(); }
			case 1: // int16
			case 2: // int32
			case 3: // int64
			{
				auto& vec = std::get< vec_vector_obj_t<int64_t> >(data);
				vec.store_at(ptr);
				break;
			}
			case 4: // float
			case 5: // double
			default:
				abort();
			}
		}

		bool are_all_adjacent_memory_addresses(unsigned elt_size) const
		{
			assert(getType() == Type::INT64);
			auto& vec = std::get< vec_vector_obj_t<int64_t> >(data);
			return vec.are_all_adjacent_memory_addresses(elt_size);
		}

		ecc::memory_address_t get_PC() const
		{
			return get_int64(0);
		}

		void setPC(ecc::memory_address_t value)
		{
			data = vec_vector_obj_t<int64_t>{ { static_cast<int64_t>(value) } };
		}

		int64_t get_int64(unsigned ix) const
		{
			assert(getType() == Type::INT64);
			auto& vec = std::get< vec_vector_obj_t<int64_t> >(data);
			return vec.get(ix);
		}

		void set_int64(unsigned ix, int64_t value)
		{
			auto& me = get_int64_array();
			return me.set(ix, value);
		}

		static VectorValue create_vec_int64_blockindex()
		{
			VectorValue ret;
			ret.data = vec_vector_obj_t<int64_t>();
			auto& arr = std::get<vec_vector_obj_t<int64_t> >(ret.data);
			arr.set_incrementing_values();
			return ret;
		}

		static VectorValue create_vec_int64(int64_t v)
		{
			VectorValue ret;
			ret.data = vec_vector_obj_t<int64_t>();
			auto& arr = std::get<vec_vector_obj_t<int64_t> >(ret.data);
			arr.replicate(v);
			return ret;
		}

		VectorValue shift_left_int64(const VectorValue& v) const
		{
			VectorValue ret(*this);
			auto& me = std::get<vec_vector_obj_t<int64_t> >(ret.data);
			const auto& other = std::get<vec_vector_obj_t<int64_t> >(v.data);
			me.shift_left(other);
			return ret;
		}

		VectorValue or_shift_left_int64(const VectorValue& v, int shift_count) const
		{
			VectorValue ret(*this);
			auto& me = std::get<vec_vector_obj_t<int64_t> >(ret.data);
			const auto& other = std::get<vec_vector_obj_t<int64_t> >(v.data);
			me.or_shift_left(other, shift_count);
			return ret;
		}


		VectorValue add_int64(const VectorValue& v) const
		{
			VectorValue ret(*this);
			auto& me = std::get<vec_vector_obj_t<int64_t> >(ret.data);
			const auto& other = std::get<vec_vector_obj_t<int64_t> >(v.data);
			me.add(other);
			return ret;
		}


		VectorValue bit_and_int64(const VectorValue& v) const
		{
			VectorValue ret(*this);
			auto& me = std::get<vec_vector_obj_t<int64_t> >(ret.data);
			const auto& other = std::get<vec_vector_obj_t<int64_t> >(v.data);
			me.bit_and(other);
			return ret;
		}

		VectorValue compare_int64(const VectorValue& v) const
		{
			VectorValue ret;
			ret.data = vec_vector_obj_t<int64_t>();

			const auto& me = std::get<vec_vector_obj_t<int64_t> >(this->data);
			const auto& other = std::get<vec_vector_obj_t<int64_t> >(v.data);
			auto& ret_cmp = std::get<vec_vector_obj_t<int64_t> >(ret.data);

			me.compare(ret_cmp, other);
			return ret;
		}

		bool all_equal_int64() const
		{
			assert(getType() == Type::INT64);
			auto& vec = std::get< vec_vector_obj_t<int64_t> >(data);
			return vec.all_equal();
		}
	};
}


namespace ecc
{
	static std::string to_string(const ecc::VectorValue& v)
	{
		return v.to_string();
	}

}