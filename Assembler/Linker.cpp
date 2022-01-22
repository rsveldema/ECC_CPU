#include "Linker.h"


std::optional<Instruction*> Linker::find_insn(const std::string& label) const
{
	auto it = code_dict.find(label);
	if (it == code_dict.end())
	{
		return std::nullopt;
	}
	return it->second;
}

std::optional<DataObject*> Linker::find_data(const std::string& label) const
{
	auto it = data_dict.find(label);
	if (it == data_dict.end())
	{
		return std::nullopt;
	}
	return it->second;

}

void Linker::link_insns(uint64_t code_address, std::vector<Instruction*>& insns)
{
	int64_t address = code_address;
	for (auto* insn : insns)
	{
		insn->address = address;

		if (insn->label != "")
		{
			code_dict[insn->label] = insn;
		}

		address += 4;
	}

	for (auto* insn : insns)
	{
		insn->link(this);
	}
}


void Linker::link_objs(uint64_t data_address, std::vector<DataObject>& objects)
{
	uint64_t address = data_address;

	for (auto& obj : objects)
	{
		obj.address = address;
		data_dict[obj.name] = &obj;

		for (auto& f : obj.fields)
		{
			f.address += address;
			address += f.size;
		}
	}
}

void Linker::link(std::vector<Instruction*>& insns, std::vector<DataObject>& objects,
	uint64_t data_address, uint64_t code_address)
{
	link_objs(data_address, objects);
	link_insns(code_address, insns);
}