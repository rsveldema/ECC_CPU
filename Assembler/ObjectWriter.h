#pragma once

#include <vector>
#include "Instruction.h"

namespace ObjectWriter
{
	void write(const std::vector<Instruction*>& insns, const std::string& filename);
}