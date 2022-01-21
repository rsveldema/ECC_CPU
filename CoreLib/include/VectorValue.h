#pragma once

#include <variant>
#include <array>

namespace Simulator
{
	static constexpr size_t VECTOR_MEM_SIZE = 64;

	template <typename ElementType>
	struct vec_vector_obj_t
	{
		using elt_t = ElementType;
		using elt_vector_t = std::array<ElementType, VECTOR_MEM_SIZE / sizeof(ElementType)>;

		elt_vector_t data;

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
					result |= MachineInfo::FLAGS_MASK_EQ;

				if (value1 > value2)
					result |= MachineInfo::FLAGS_MASK_GT;

				if (value1 < value2)
					result |= MachineInfo::FLAGS_MASK_LT;

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

		int64_t get(unsigned ix) const
		{
			assert(ix < data.size());
			return data[ix];
		}

		bool are_all_adjacent_memory_addresses(unsigned elt_size) const
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
			for (int i = 0; i < data.size(); i++)
			{
				ret |= (data[i] != 0) << i;
			}
			return ret;
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


		uint64_t reduce_int64_to_single_int64_t() const
		{
			auto& vec = std::get< vec_vector_obj_t<int64_t> >(data);
			return vec.reduce_to_uint64_t();
		}

		void load_from_int64(uint8_t* ptr)
		{
			data = vec_vector_obj_t<int64_t>{ { } };
			auto& vec = std::get< vec_vector_obj_t<int64_t> >(data);
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
			const auto& me = std::get<vec_vector_obj_t<int64_t> >(data);
			return me.are_all_adjacent_memory_addresses(elt_size);
		}

		MachineInfo::memory_address_t get_PC() const
		{
			return get_int64(0);
		}

		void setPC(MachineInfo::memory_address_t value)
		{
			data = vec_vector_obj_t<int64_t>{ { static_cast<int64_t>(value) } };
		}

		int64_t get_int64(unsigned ix) const
		{
			const auto& me = std::get<vec_vector_obj_t<int64_t> >(data);
			return me.get(0);
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
			const auto& me = std::get<vec_vector_obj_t<int64_t> >(this->data);
			return me.all_equal();
		}
	};
}
