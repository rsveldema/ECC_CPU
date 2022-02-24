#pragma once

#include <optional>
#include <iostream>

#include "RegisterFile.h"
#include "Defines.h"

namespace ecc
{
	struct ThreadContext
	{
		RegisterFile regs;
		ecc::memory_address_t PC{ 0 };
		execution_mask_t exec_mask;
	};


	class DivergenceQueue
	{
	public:
		DivergenceQueue()
		{
		}

		void init()
		{}

		bool is_empty() const
		{
			return read_pos == write_pos;
		}

		void push_front(const ThreadContext& ctxt)
		{
			auto next_write_pos = get_next_pos(write_pos);
			if (next_write_pos == read_pos)
			{
				std::cerr << "DivergenceQueue is FULL!" << std::endl;
				abort();
			}
			contexts[write_pos] = ctxt;
			write_pos = next_write_pos;
		}

		std::optional<ThreadContext> pop_back()
		{
			if (is_empty())
			{
				return std::nullopt;
			}
			const auto& v = contexts[read_pos];
			read_pos = get_next_pos(read_pos);
			return v;
		}

	private:

		static constexpr uint32_t MAX_THREAD_CONTEXTS = 64;

		ThreadContext contexts[MAX_THREAD_CONTEXTS];
		unsigned read_pos = 0;
		unsigned write_pos = 0;



		static uint32_t get_next_pos(uint32_t v)
		{
			if (v == (MAX_THREAD_CONTEXTS - 1))
			{
				return 0;
			}
			else
			{
				return v + 1;
			}
		}

	};

}