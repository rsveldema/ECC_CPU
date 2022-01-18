#include "Task.h"


namespace coro
{
		void Task::await_suspend(std::coroutine_handle<> h) 
		{
			h_ = h;
			suspended = true;
		}
		
		void Task::resume()
		{
			assert(suspended);
			h_();
		}
}
