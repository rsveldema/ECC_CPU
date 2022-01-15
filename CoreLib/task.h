#pragma once


#include <coroutine>
#include <iostream>
#include <cassert>

namespace coro
{
	struct ReturnObject {
		struct promise_type {
			ReturnObject get_return_object() { return {}; }
			std::suspend_never initial_suspend() { return {}; }
			std::suspend_never final_suspend() noexcept { return {}; }
			void return_void() {}
			void unhandled_exception() {}
		};
	};



	class Task
	{
	public:
		bool suspended = false;
		std::coroutine_handle<> h_;
		constexpr bool await_ready() const noexcept { return false; }
		void await_suspend(std::coroutine_handle<> h) { h_ = h; suspended = true; }
		constexpr void await_resume() const noexcept {}


		void resume()
		{
			assert(suspended);
			h_();
		}
	};
}
