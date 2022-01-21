#pragma once

#include <string>
#include <map>
#include "Instruction.h"

class Linker;

class Instruction
{
public:
	std::string label;
	uint64_t address = 0;

	virtual uint32_t getEncodedInstruction() = 0;
	virtual void link(const Linker* linker) = 0;
};