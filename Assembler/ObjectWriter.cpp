#include "ObjectWriter.h"
#include <iostream>
#include <fstream>


namespace ObjectWriter
{
	void write(const std::vector<DataObject>& dataList, const std::string& filename)
	{
		std::ofstream myfile;
		myfile.open(filename, std::ios::binary | std::ios::trunc | std::ios::out);
		if (!myfile.is_open())
		{
			std::cerr << "failed to open file " << filename << std::endl;
			return;
		}

		std::vector<uint8_t> values;

		for (const auto& obj : dataList)
		{
			obj.write(values);
		}

		myfile.write((char*)values.data(), values.size());
		myfile.close();
	}

	void write(const std::vector<Insns::Instruction*>& insns, const std::string& filename)
	{
		std::ofstream myfile;
		myfile.open(filename, std::ios::binary | std::ios::trunc | std::ios::out);
		if (!myfile.is_open())
		{
			std::cerr << "failed to open file " << filename << std::endl;
			return;
		}

		for (auto* insn : insns)
		{
			auto encoded = insn->getEncodedInstruction();
			const char* data = reinterpret_cast<char*>(&encoded);
			myfile.write(data, sizeof(encoded));
		}

		myfile.close();

	}
}