#pragma once


struct Cycles {
	uint64_t cycles;

	Cycles() : cycles(0) {}

	explicit Cycles(uint64_t c) : cycles(c) {}

	Cycles& operator++(int)
	{
		cycles++;
		return *this;
	}
};