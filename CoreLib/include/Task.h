#pragma once


#include <coroutine>
#include <iostream>
#include <cassert>


namespace coro
{


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

		Task(const std::string& _name)
			: name(_name)
		{}

		Task(const Task& ) = delete;
		Task(const Task&& ) = delete;

		void operator = (const Task& t) = delete;

		Task()
		{
			dead = true;
		}

		std::coroutine_handle<> h_;
		
		constexpr bool await_ready() const noexcept 
		{
			return false; 
		}
		
		void await_suspend(std::coroutine_handle<> h);
		
		constexpr void await_resume() const noexcept 
		{
		}

		void resume();
	};

	/** C++ coroutines require the return type to hold a nested promise_type.
	*/
	struct ReturnObject {
		struct promise_type {
			ReturnObject get_return_object() { return {}; }
			std::suspend_never initial_suspend() { return {}; }
			std::suspend_never final_suspend() noexcept { return {}; }
			void return_void() {}
			void unhandled_exception() {}
		};
	};
}
