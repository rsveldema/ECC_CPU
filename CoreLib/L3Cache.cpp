#include "CoreLib.h"


namespace Simulator
{
	coro::ReturnObject L3Cache::run()
	{
		running = true;
		while (1)
		{
			if (!toMemory.is_busy())
			{
				if (auto pkt = toCPU.try_accept_request())
				{
					toMemory.send_request(*pkt);
				}
			}

			if (auto pkt = toMemory.try_accept_response())
			{
				toCPU.send_response(*pkt);
			}

			Task& t = *this;
			co_await t;
		}
		running = false;
	}
}