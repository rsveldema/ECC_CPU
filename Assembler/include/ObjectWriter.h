#pragma once

#include <vector>

#include "Instruction.h"
#include "DataObject.h"

namespace ObjectWriter
{
	void write(const std::vector<Insns::Instruction*>& insns, const std::string& filename);
	void write(const std::vector<DataObject>& insns, const std::string& filename);
}