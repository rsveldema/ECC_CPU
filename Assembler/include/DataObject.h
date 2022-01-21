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
		size = sizeof(value);
	}

	void write(std::vector<uint8_t>& values) const
	{
		assert(address >= MachineInfo::DATA_SEGMENT_START);
		auto end = address + size;
		if (values.size() < end)
		{
			values.resize(end);
		}

		switch (value.index())
		{
		case 0:
		{
			auto tmp = std::get<int8_t>(value);
			write_value(values, tmp);
			break;
		}
		case 1:
		{
			auto tmp = std::get<int16_t>(value);
			write_value(values, tmp);
			break;
		}
		case 2:
		{
			auto tmp = std::get<int32_t>(value);
			write_value(values, tmp);
			break;
		}
		case 3:
		{
			auto tmp = std::get<int64_t>(value);
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
			values[i + address] = begin[i];
		}
	}
};

struct DataObject
{
	std::string name;
	std::vector<DataField> fields;


	void write(std::vector<uint8_t>& values) const
	{
		for (const auto& obj : fields)
		{
			obj.write(values);
		}
	}
};
