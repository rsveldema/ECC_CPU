#pragma once

#include <vector>
#include "Instruction.h"

class Linker
{
public:
	void link(std::vector<Instruction*>& insns);
};