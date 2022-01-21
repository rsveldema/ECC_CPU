#pragma once

#include <string>
#include "CoreLib.h"

#include "Mnemonics.h"

#include "SourcePosition.h"
#include "DataObject.h"

#include "Instruction.h"
#include "MemoryAccess.h"
#include "Halt.h"
#include "Jmp.h"
#include "AddRegRegConst.h"
#include "AddRegRegReg.h"
#include "MoveRegConst.h"
#include "MoveRegReg.h"
#include "CmpRegReg.h"
#include "RestorePC.h"
#include "StoreRegister.h"
#include "LoadRegister.h"
#include "MoveRegSymbol.h"
#include "ShlRegRegConst.h"
#include "MoveRegBlockIndex.h"
#include "LoadAddr.h"

class Line
{
public:
	std::string data;

	bool StartsWith(const std::string& s) const;
	bool EndsWith(const std::string& s) const;

	// remove whitespace and comments
	void trim();

	char operator[](unsigned ix) const
	{
		assert(ix < data.size());
		return data[ix];
	}
};

class Program
{
public:
	void parse(const std::string& filename);
	std::vector<Instruction*>& getInsns()
	{
		return insns;
	}

	std::vector<DataObject>& getObjects()
	{
		return objects;
	}

private:
	std::vector<DataObject> objects;
	std::vector<Instruction*> insns;

	std::string lastSeenLabel;
	void parseCodeLine(const Line& line, const SourcePosition& pos);
	void parseDataLine(const Line& line, const SourcePosition& pos);

	void Add(Instruction* insn);
};