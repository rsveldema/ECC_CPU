#pragma once


namespace Simulator
{
	class ExecutionMask
	{
	public:
		ExecutionMask() : flags(0)
		{}

		explicit ExecutionMask(uint64_t _flags) : flags(_flags)
		{}

	private:
		uint64_t flags;
	};
}