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
	void link(std::vector<Insns::Instruction*>& insns, std::vector<DataObject>& objects,
		uint64_t data_address, uint64_t code_address);

	std::optional<Insns::Instruction*> find_insn(const std::string& label) const;
	std::optional<DataObject*> find_data(const std::string& label) const;

private:
	std::map<std::string, DataObject*> data_dict;
	std::map<std::string, Insns::Instruction*> code_dict;

	void link_insns(uint64_t code_address, std::vector<Insns::Instruction*>& insns);
	void link_objs(uint64_t data_address, std::vector<DataObject>& objects);
};