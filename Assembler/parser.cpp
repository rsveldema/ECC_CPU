#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <climits>

#include "Assembler.h"
#include "DataObject.h"

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

	const std::string CODE_LINE_SPLIT_CHARS = ",.\t ";
	const std::string DATA_LINE_SPLIT_CHARS = ",\t ";


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

	Mnemonic stringToOpcode(const std::string& opcodeName, const SourcePosition& pos)
	{
		if (opcodeName == "loadaddress_lo")
			return Mnemonic::LOAD_ADDR_LO;

		if (opcodeName == "loadaddress_mid")
			return Mnemonic::LOAD_ADDR_MID;

		if (opcodeName == "loadaddress_hi")
			return Mnemonic::LOAD_ADDR_HI;

		if (opcodeName == "nop")
			return Mnemonic::NOP;

		if (opcodeName == "shl")
			return Mnemonic::SHL;

		if (opcodeName == "halt")
			return Mnemonic::HALT;

		if (opcodeName == "mov")
			return Mnemonic::MOV;

		if (opcodeName == "add")
			return Mnemonic::ADD;

		if (opcodeName == "jmp")
			return Mnemonic::JMP;

		if (opcodeName == "restorepc")
			return Mnemonic::LOAD_RESTORE_PC;

		if (opcodeName == "store")
			return Mnemonic::STORE;

		if (opcodeName == "load")
			return Mnemonic::LOAD;

		if (opcodeName == "cmp")
			return Mnemonic::CMP;

		if (opcodeName == "je")
			return Mnemonic::JE;

		if (opcodeName == "jne")
			return Mnemonic::JNE;

		if (opcodeName == "jg")
			return Mnemonic::JG;

		if (opcodeName == "jge")
			return Mnemonic::JGE;

		if (opcodeName == "jl")
			return Mnemonic::JL;

		if (opcodeName == "jle")
			return Mnemonic::JLE;

		pos.error("unrecognized insn in string2opcode: " + opcodeName);
		return Mnemonic::NOP;
	}
}

bool isRegister(const std::string& str)
{
	if (ecc::regnames.find(str) != ecc::regnames.end())
	{
		return true;
	}
	return false;
}

ecc::RegisterID getRegisterID(const std::string& str)
{
	return ecc::regnames[str];
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


void Program::parseDataLine(const Line& line, const SourcePosition& pos)
{
	std::cerr << "PARSE-DATA: " << line.data << std::endl;
	if (line.EndsWith(":"))
	{
		lastSeenLabel = line.data.substr(0, line.data.size() - 1);
		this->objects.push_back({ lastSeenLabel });
		return;
	}

	auto toks = tokenize(line.data, DATA_LINE_SPLIT_CHARS);
	if (toks[0] == ".long")
	{
		int64_t value = std::stol(toks[1]);
		if (this->objects.size() > 0)
		{
			DataField fld{ value };
			this->objects[this->objects.size() - 1].fields.push_back(fld);
		}
		else
		{
			pos.error("data field without label" + line.data);
		}
	}
	else
	{
		pos.error("unhandled data case: " + line.data);
	}
}

void Program::parseCodeLine(const Line& line, const SourcePosition& pos)
{
	std::cerr << "PARSE-INSN: " << line.data << std::endl;

	if (line.EndsWith(":"))
	{
		lastSeenLabel = line.data.substr(0, line.data.size() - 1);
		return;
	}

	auto toks = tokenize(line.data, CODE_LINE_SPLIT_CHARS);
	switch (auto op = stringToOpcode(toks[0], pos))
	{
	case Mnemonic::HALT:
	{
		Add(new Halt());
		break;
	}

	case Mnemonic::LOAD_ADDR_LO:
	{
		Add(new LoadAddr(LoadAddr::Type::LO, toks[2], pos));
		break;
	}
	case Mnemonic::LOAD_ADDR_MID:
	{
		Add(new LoadAddr(LoadAddr::Type::MID, toks[2], pos));
		break;
	}
	case Mnemonic::LOAD_ADDR_HI:
	{
		Add(new LoadAddr(LoadAddr::Type::HI, toks[2], pos));
		break;
	}

	case Mnemonic::JMP:
	case Mnemonic::JE:
	case Mnemonic::JNE:
	case Mnemonic::JG:
	case Mnemonic::JGE:
	case Mnemonic::JL:
	case Mnemonic::JLE:
	{
		const auto& l = toks[1];
		Add(new Jmp(l, pos, op));
		break;
	}


	case Mnemonic::CMP:
	{
		if (isRegister(toks[1]) && isRegister(toks[2]))
		{
			Add(new CmpRegReg(getRegisterID(toks[1]), getRegisterID(toks[2])));
		}
		else
		{
			pos.error(std::string("failed to find a matching cmp insn for: ") + line.data);
		}
		break;
	}

	case Mnemonic::SHL:
	{
		if (isRegister(toks[1]) && isRegister(toks[2]) && isInt8Constant(toks[3]))
		{
			Add(new ShlRegRegConst(getRegisterID(toks[1]), getRegisterID(toks[2]), getInt8Constant(toks[3])));
		}
		else
		{
			pos.error(std::string("failed to find a matching mov insn for: ") + line.data);
		}
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
			auto src = getRegisterID(toks[2]);
			if (src == ecc::RegisterID::BLOCK_INDEX)
			{
				Add(new MoveRegBlockIndex(getRegisterID(toks[1]), getRegisterID(toks[2])));
			}
			else
			{
				Add(new MoveRegReg(getRegisterID(toks[1]), getRegisterID(toks[2])));
			}
		}
		else if (isRegister(toks[1]) && isSymbol(toks[2]))
		{
			Add(new MoveRegSymbol(getRegisterID(toks[1]), toks[2], pos));
		}
		else
		{
			pos.error(std::string("failed to find a matching mov insn for: ") + line.data);
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

	case Mnemonic::LOAD:
	{
		auto dest_reg = getRegisterID(toks[1]);
		auto src_memory_access = parse_memory_access(toks[2]);
		//  0(%sp), %r0
		Add(new LoadRegister(dest_reg, src_memory_access.base_reg, src_memory_access.offset));
		break;
	}

	case Mnemonic::LOAD_RESTORE_PC:
	{
		auto memory_access = parse_memory_access(toks[1]);
		Add(new RestorePC(memory_access.base_reg, memory_access.offset));
		break;
	}

	case Mnemonic::ADD:
	{
		if (toks.size() != 4)
		{
			pos.error("need 3 operands to " + line.data);
		}
		else if (isRegister(toks[1]) && isRegister(toks[2]) && isInt8Constant(toks[3]))
		{
			Add(new AddRegRegConst(getRegisterID(toks[1]), getRegisterID(toks[2]), getInt8Constant(toks[3])));
		}
		else if (isRegister(toks[1]) && isRegister(toks[2]) && isRegister(toks[3]))
		{
			Add(new AddRegRegReg(getRegisterID(toks[1]), getRegisterID(toks[2]), getRegisterID(toks[3])));
		}
		else
		{
			pos.error("failed to find a matching add insn for: " + line.data);
		}
		break;
	}

	default:
		pos.error("unhandled insn case: " + line.data);
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

	for (int i = 0; i < data.size(); i++)
	{
		if (data[i] == '/' || data[i] == '#')
		{
			data = data.substr(0, i);
			break;
		}
	}
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

		if (data_mode)
		{
			parseDataLine(line, pos);
		}
		else
		{
			parseCodeLine(line, pos);
		}
	}
}