#pragma once

#include <string>
#include <map>
#include "Instruction.h"

class Instruction
{
public:
	std::string label;
	uint64_t address = 0;

	virtual uint32_t getEncodedInstruction() = 0;
	virtual void link(const std::map<std::string, Instruction*>& dict) = 0;
};