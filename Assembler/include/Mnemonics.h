#pragma once


enum class Mnemonic
{
	NOP,
	HALT,
	CMP,
	MOV,
	ADD,
	JMP,
	STORE,
	LOAD_RESTORE_PC,
	LOAD,
	JE,
	JNE,
	JG,
	JGE,
	JL,
	JLE,
	SHL,


	LOAD_ADDR_LO,
	LOAD_ADDR_MID,
	LOAD_ADDR_HI
};
