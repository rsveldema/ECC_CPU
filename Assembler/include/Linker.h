#pragma once

#include <vector>
#include <optional>

#include "Instruction.h"
#include "DataObject.h"

namespace Insns
{
	class Instruction;
}


class Linker
{
public:
	void link(std::vector<Instruction*>& insns, std::vector<DataObject>& objects,
		uint64_t data_address, uint64_t code_address);

	std::optional<Instruction*> find_insn(const std::string& label) const;
	std::optional<DataObject*> find_data(const std::string& label) const;

private:
	std::map<std::string, DataObject*> data_dict;
	std::map<std::string, Instruction*> code_dict;

	void link_insns(uint64_t code_address, std::vector<Instruction*>& insns);
	void link_objs(uint64_t data_address, std::vector<DataObject>& objects);
};