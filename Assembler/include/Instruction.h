#pragma once

#include <string>
#include <map>

class Linker;

namespace Insns
{
	class Instruction
	{
	public:
		std::string label;
		uint64_t address = 0;

		virtual uint32_t getEncodedInstruction() = 0;
		virtual void link(const Linker* linker) = 0;
	};
}