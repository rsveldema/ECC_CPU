#pragma once

#include <optional>
#include <iostream>

#include "RegisterFile.h"
#include "Defines.h"

namespace ecc
{
	static constexpr uint32_t MAX_THREAD_CONTEXTS = 64;

	struct ThreadContext
	{
		RegisterFile regs;
		memory_address_t PC;
		execution_mask_t exec_mask;
	};


	INTERFACE DivergenceQueue
	{
		std::array<ThreadContext, MAX_THREAD_CONTEXTS> contexts;
		uint16_t read_pos;
		uint16_t write_pos;


		METHOD_SECTION;

		void init()
		{
			read_pos = 0;
			write_pos = 0;
		}

		bool is_empty() const
		{
			return read_pos == write_pos;
		}

		void push_front(RegisterFile regs,
						memory_address_t PC,
						execution_mask_t exec_mask)
		{
			if (get_next_pos(write_pos) == read_pos)
			{
				$display("DivergenceQueue is FULL!");
				assert(false);
			}
			contexts[write_pos].exec_mask = exec_mask;
			contexts[write_pos].PC = PC;
			contexts[write_pos].regs = regs;
			
			write_pos = get_next_pos(write_pos);
		}

		ThreadContext get_back()
		{
			assert(! is_empty());

			return contexts[read_pos];
		}


		void advance_read_pos()
		{
			read_pos = get_next_pos(read_pos);
		}

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