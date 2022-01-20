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
	};

	struct VectorValue
	{
		std::variant<vec_vector_obj_t<int8_t>,
			vec_vector_obj_t<int16_t>,
			vec_vector_obj_t<int32_t>,
			vec_vector_obj_t<int64_t>,

			vec_vector_obj_t<float>,
			vec_vector_obj_t<double>> data;


		MachineInfo::memory_address_t get_PC() const
		{
			return std::get< vec_vector_obj_t<int64_t> >(data).data[0];
		}

		void setPC(MachineInfo::memory_address_t value)
		{
			data = vec_vector_obj_t<int64_t>{ { static_cast<int64_t>(value) } };
		}

		static VectorValue replicate_int64(int64_t v)
		{
			VectorValue ret;
			ret.data = vec_vector_obj_t<int64_t>();
			auto& arr = std::get<vec_vector_obj_t<int64_t> >(ret.data);
			arr.replicate(v);
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
	};
}
