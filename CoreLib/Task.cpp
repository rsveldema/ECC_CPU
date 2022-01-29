#include "Task.h"

namespace ecc
{
	void Task::await_suspend(std::coroutine_handle<ReturnObject::promise_type> h)
	{
		h_ = h;
		suspended = true;
	}

	void Task::resume()
	{
		assert(magic == (void*)(uint64_t)0xdeadbeef);
		assert(suspended);

		if (h_.promise().exception_)
		{
			std::rethrow_exception(h_.promise().exception_);
		}

		h_();
	}
}
