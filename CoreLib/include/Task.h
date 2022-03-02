#pragma once


#include <coroutine>
#include <iostream>
#include <cassert>
#include <string>
#include "Defines.h"

#include "GlobalStats.h"

namespace ecc
{
	using phys_memory_address_t = uint32_t;

	static inline
	void dump(GlobalStats& stats)
	{
		std::cout << "-----------------<<[  Stats ]>>------------------" << std::endl;
		std::cout << "Total #cycles                   : " << std::to_string(stats.numCyclesUsed) << std::endl;
		std::cout << "wait #wait cycles in fetch-insn : " << std::to_string(stats.waitForInsnFetch) << std::endl;
		std::cout << "wait #wait cycles in fetch-op   : " << std::to_string(stats.waitForOperandFetch) << std::endl;
		std::cout << "numVectorLocalDivergences       : " << std::to_string(stats.numVectorLocalDivergences) << std::endl;
	}



#define SET_BIT(ret, bitnum, value)    ret |= ((value) << bitnum)

#define PACK8(a0, a1, a2, a3, a4, a5, a6, a7)    ( \
	((uint64_t)a0) << 0 |  \
	((uint64_t)a1) << 8 |  \
	((uint64_t)a2) << 16 | \
	((uint64_t)a3) << 24 | \
	((uint64_t)a4) << 32 | \
	((uint64_t)a5) << 40 | \
	((uint64_t)a6) << 48 | \
	((uint64_t)a7) << 56)

	#define CONTEXT_SWITCH() { Task& t = *this; co_await t; }
	#define WAIT(cond) while (! cond) CONTEXT_SWITCH()

	#define READ_MEMORY_DELAY() { \
        for (uint64_t i = 0; i < DRAM_READ_ACCESS_CYCLES; i++) { \
            CONTEXT_SWITCH(); \
        } \
	}

	#define WRITE_MEMORY_DELAY() { \
        for (uint64_t i = 0; i < DRAM_WRITE_ACCESS_CYCLES; i++) { \
            CONTEXT_SWITCH(); \
        } \
	}
			
	/** C++ coroutines require the return type to hold a nested promise_type.
	*/
	struct ReturnObject {
		struct promise_type {
			std::exception_ptr exception_ = nullptr;

			ReturnObject get_return_object() { return {}; }
			std::suspend_never initial_suspend() { return {}; }
			std::suspend_never final_suspend() noexcept { return {}; }
			void return_void() {}
			void unhandled_exception() { exception_ = std::current_exception(); }
		};
	};


	/** active components (simulated things that do stuff)
	* should inherit from this so that we can semi-parallel execute them using co-routines.
	*  - When calling 'co_await self()', it calls await_resume() and then suspends itself.
	*  - when calling 'resume()' the coroutine is restarted (=called).
	*/
	class Task
	{
	public:
		bool suspended = false;
		const std::string name;
		bool dead = false;
		bool running = false;
		void* magic = (void*)(uint64_t)0xdeadbeef;


		Task(const std::string& _name)
			: name(_name)
		{}

		Task(const Task&) = delete;
		Task(const Task&&) = delete;

		void operator = (const Task& t) = delete;

		Task()
		{
			dead = true;
		}

		std::coroutine_handle<ReturnObject::promise_type> h_;

		constexpr bool await_ready() const noexcept
		{
			return false;
		}

		void await_suspend(std::coroutine_handle<ReturnObject::promise_type> h);

		constexpr void await_resume() const noexcept
		{
		}

		void resume();
	};
}
