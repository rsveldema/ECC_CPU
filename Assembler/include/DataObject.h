#pragma once

#include <variant>
#include <vector>
#include "MachineInfo.h"
#include <cassert>

struct DataField
{
	std::variant<int8_t, int16_t, int32_t, int64_t> value;
	uint64_t size;
	MachineInfo::memory_address_t address = 0;

	DataField(int64_t _value)
		: value(_value)
	{
		size = sizeof(_value);
	}

	void write(std::vector<uint8_t>& values) const
	{
		assert(address >= MachineInfo::DATA_SEGMENT_START);

		switch (value.index())
		{
		case 0:
		{
			int8_t tmp = std::get<int8_t>(value);
			write_value(values, tmp);
			break;
		}
		case 1:
		{
			int16_t tmp = std::get<int16_t>(value);
			write_value(values, tmp);
			break;
		}
		case 2:
		{
			int32_t tmp = std::get<int32_t>(value);
			write_value(values, tmp);
			break;
		}
		case 3:
		{
			int64_t tmp = std::get<int64_t>(value);
			write_value(values, tmp);
			break;
		}
		}
	}

	template <typename T>
	void write_value(std::vector<uint8_t>& values, const T t) const
	{
		const auto* begin = (const char*)&t;
		const auto* end = begin + sizeof(t);

		for (int i = 0; i < sizeof(t); i++)
		{
			auto write_ix = (i + address) - MachineInfo::DATA_SEGMENT_START;

			values.resize(write_ix + 1);

			values[write_ix] = begin[i];
		}
	}
};

struct DataObject
{
	std::string name;
	std::vector<DataField> fields;
	uint64_t address = 0;


	void write(std::vector<uint8_t>& values) const
	{
		for (const auto& obj : fields)
		{
			obj.write(values);
		}
	}

	uint64_t getAddress() const { return address; }
};
