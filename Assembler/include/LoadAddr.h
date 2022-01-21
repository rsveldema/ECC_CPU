#pragma once

#include "Instruction.h"
#include <map>

namespace Insns
{
	class LoadAddr : public Instruction
	{
	public:

		enum class Type {
			LO, MID, HI
		};

	private:
		const Type type;
		const std::string global_var_name;
		uint32_t address = 0;
		const SourcePosition pos;

	public:
		LoadAddr(Type _type, const std::string& _global_var_name, const SourcePosition& _pos)
			: type(_type), global_var_name(_global_var_name), pos(_pos)
		{
		}

		uint32_t getEncodedInstruction() override
		{
			uint64_t mask = 0xffffff;

			switch (type)
			{
			case Type::LO:
			{
				address >>= 0;
				address &= mask;
				return ((uint32_t)MachineInfo::Opcode::MOVE_R0_CONST24A) | ((uint32_t)address << 8);
			}
			case Type::MID: {
				address >>= 24;
				address &= mask;
				return ((uint32_t)MachineInfo::Opcode::MOVE_R0_CONST24B) | ((uint32_t)address << 8);
			}

			case Type::HI: {
				address >>= 48;
				address &= mask;
				return ((uint32_t)MachineInfo::Opcode::MOVE_R0_CONST24C) | ((uint32_t)address << 8);
			}
			}
			return 0;
		}

		void link(const Linker* linker) override;
	};
}