#pragma once


namespace ecc
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

		uint32_t count() const
		{
			uint32_t c = 0;
			uint64_t f = flags;
			for (unsigned i = 0; i < 64; i++)
			{
				if (f & 1)
				{
					c++;
				}
				f >>= 1;
			}
			return c;
		}

	private:
		uint64_t flags;
	};
}