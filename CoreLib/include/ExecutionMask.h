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

		uint64_t get_masked_flags(uint64_t f) const
		{
			return flags & f;
		}

	private:
		uint64_t flags;
	};
}