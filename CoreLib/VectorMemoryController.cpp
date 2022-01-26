#include "CoreLib.h"


namespace Simulator
{
	coro::ReturnObject VectorMemoryController::run()
	{
		while (1)
		{


			Task& t = *this;
			co_await t;
		}
	}
}
