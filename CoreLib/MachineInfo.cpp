#include "MachineInfo.h"

namespace MachineInfo
{
	std::map<std::string, InstructionInfo> insnInfo;

	std::map<std::string, Register> regnames {
		{ "%r0", Register::R0 },
		{ "%r1", Register::R1 },
		{ "%r2", Register::R2 },
		{ "%r3", Register::R3 },
		{ "%r4", Register::R4 },
		{ "%r5", Register::R5 },
		{ "%r6", Register::R6 },
		{ "%r7", Register::R7 },
		{ "%r8", Register::R8 },
		{ "%sp", Register::SP },
		{ "%flags", Register::FLAGS },
	};

}
