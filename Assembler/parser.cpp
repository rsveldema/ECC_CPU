#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <climits>

#include "Assembler.h"

using namespace Insns;


namespace
{
	bool isSplitChar(char c, const std::string& splitChars)
	{
		for (int i = 0; i < splitChars.size(); i++)
		{
			if (c == splitChars[i])
			{
				return true;
			}
		}
		return false;
	}

	const std::string LINE_SPLIT_CHARS = ",.\t ";


	std::vector<std::string> tokenize(const std::string& data, const std::string& splitChars)
	{
		std::vector<std::string> ret;
		const char* s = data.c_str();
		do
		{
			const char* tokBegin = s;
			while (!isSplitChar(*s, splitChars) && *s)
			{
				s++;
			}

			const char* tokEnd = s;
			if (tokBegin != tokEnd)
			{
				ret.push_back(std::string(tokBegin, tokEnd));
			}
		} while (*s++);

		return ret;
	}
}

void Program::Add(Instruction* insn)
{
	insn->label = lastSeenLabel;
	lastSeenLabel = "";
	insns.push_back(insn);
}



namespace {

	enum class Mnemonic
	{
		NOP,
		HALT,
		MOV,
		ADD,
		JMP,
		STORE,
		RESTORE_PC
	};

	Mnemonic stringToOpcode(const std::string& opcodeName, const SourcePosition& pos)
	{
		if (opcodeName == "nop")
			return Mnemonic::NOP;

		if (opcodeName == "halt")
			return Mnemonic::HALT;

		if (opcodeName == "mov")
			return Mnemonic::MOV;

		if (opcodeName == "add")
			return Mnemonic::ADD;

		if (opcodeName == "jmp")
			return Mnemonic::JMP;

		if (opcodeName == "restorepc")
			return Mnemonic::RESTORE_PC;

		if (opcodeName == "store")
			return Mnemonic::STORE;

		pos.error("unrecognized insn: " + opcodeName);
		return Mnemonic::NOP;
	}
}

bool isRegister(const std::string& str)
{
	if (MachineInfo::regnames.find(str) != MachineInfo::regnames.end())
	{
		return true;
	}
	return false;
}

MachineInfo::Register getRegisterID(const std::string& str)
{
	return MachineInfo::regnames[str];
}

bool isInt16Constant(const std::string& str)
{
	try {
		size_t ix = 0;
		int value = std::stoi(str, &ix);
		if (ix != str.size())
		{
			return false;
		}
		if (value < SHRT_MIN || value > SHRT_MAX)
		{
			return false;
		}
		return true;
	}
	catch (const std::invalid_argument&)
	{
		return false;
	}
}

bool isInt8Constant(const std::string& str)
{
	try {
		size_t ix = 0;
		int value = std::stoi(str, &ix);
		if (ix != str.size())
		{
			return false;
		}
		if (value < CHAR_MIN || value > CHAR_MAX)
		{
			return false;
		}
		return true;
	}
	catch (const std::invalid_argument&)
	{
		return false;
	}
}


int16_t getInt16Constant(const std::string& str)
{
	int value = std::stoi(str);
	return value;
}

int8_t getInt8Constant(const std::string& str)
{
	int value = std::stoi(str);
	return value;
}

bool isSymbol(const std::string& str)
{
	return isalpha(str[0]) || str[0] == '_' || str[0] == '$';
}


MemoryAccess parse_memory_access(const std::string& t1)
{
	auto s = tokenize(t1, "()");

	auto off = getInt16Constant(s[0]);
	auto reg = getRegisterID(s[1]);

	return MemoryAccess(reg, off);
}



void Program::parseLine(const Line& line, const SourcePosition& pos)
{
	std::cerr << "PARSE: " << line.data << std::endl;

	if (line.EndsWith(":"))
	{
		lastSeenLabel = line.data.substr(0, line.data.size() - 1);
		return;
	}

	auto toks = tokenize(line.data, LINE_SPLIT_CHARS);
	switch (stringToOpcode(toks[0], pos))
	{
	case Mnemonic::HALT:
	{
		Add(new Halt());
		break;
	}

	case Mnemonic::JMP:
	{
		auto l = toks[1];
		Add(new Jmp(l, pos));
		break;
	}

	case Mnemonic::MOV:
	{
		if (isRegister(toks[1]) && isInt16Constant(toks[2]))
		{
			Add(new MoveRegConst(getRegisterID(toks[1]), getInt16Constant(toks[2])));
		}
		else if (isRegister(toks[1]) && isRegister(toks[2]))
		{
			Add(new MoveRegReg(getRegisterID(toks[1]), getRegisterID(toks[2])));
		}
		else if (isRegister(toks[1]) && isSymbol(toks[2]))
		{
			Add(new MoveRegSymbol(getRegisterID(toks[1]), toks[2], pos));
		}
		else
		{
			pos.error(std::string("failed to find a matching insn for: ") + line.data);
		}
		break;
	}

	case Mnemonic::STORE:
	{
		auto memory_access = parse_memory_access(toks[1]);
		auto src_reg = getRegisterID(toks[2]);

		//  0(%sp), %r0

		Add(new StoreRegister(memory_access.base_reg, memory_access.offset, src_reg));
		break;
	}

	case Mnemonic::RESTORE_PC:
	{
		auto memory_access = parse_memory_access(toks[1]);
		Add(new RestorePC(memory_access.base_reg, memory_access.offset));
		break;
	}

	case Mnemonic::ADD:
	{
		if (isRegister(toks[1]) && isRegister(toks[2]) && isInt8Constant(toks[3]))
		{
			Add(new AddRegRegConst(getRegisterID(toks[1]), getRegisterID(toks[2]), getInt8Constant(toks[3])));
		}
		else if (isRegister(toks[1]) && isRegister(toks[2]) && isRegister(toks[3]))
		{
			Add(new AddRegRegReg(getRegisterID(toks[1]), getRegisterID(toks[2]), getRegisterID(toks[3])));
		}
		else
		{
			pos.error("failed to find a matching insn for: " + line.data);
		}
		break;
	}

	default:
		pos.error("unhandled case: " + line.data);
	}
}


void Line::trim()
{
	int start = 0;
	while (start < data.size())
	{
		if (!isspace(data[start]))
		{
			break;
		}
		start++;
	}

	size_t count = data.size() - start;
	data = data.substr(start, count);
}

bool Line::StartsWith(const std::string& prefix) const
{
	if (data.size() < prefix.size())
	{
		return false;
	}
	return data.substr(0, prefix.size()) == prefix;
}

bool Line::EndsWith(const std::string& postfix) const
{
	if (data.size() < postfix.size())
	{
		return false;
	}
	return data.substr(data.size() - postfix.size(), postfix.size()) == postfix;
}

void Program::parse(const std::string& filename)
{
	std::ifstream infile(filename);

	if (!infile.is_open())
	{
		std::cerr << "failed to open file: " + filename << std::endl;
		return;
	}

	std::string rawline;
	int lineNumber = 0;
	bool data_mode = false;
	while (std::getline(infile, rawline))
	{
		lineNumber++;

		Line line(rawline);
		line.trim();

		if (line.data.size() == 0)
		{
			continue;
		}

		if (line[0] == '#' || line[0] == '/')
		{
			continue;
		}

		if (line.StartsWith(".data"))
		{
			data_mode = true;
			continue;
		}

		if (line.StartsWith(".code"))
		{
			data_mode = false;
			continue;
		}

		SourcePosition pos{ lineNumber, filename };
		parseLine(line, pos);
	}
}