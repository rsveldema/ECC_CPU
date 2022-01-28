#include "CoreLib.h"


namespace ecc
{
	ecc::ReturnObject L1InsnCache::run()
	{
		running = true;
		while (1)
		{
			if (auto pkt = toCPU.try_accept_request())
			{
				toMemory.send_request(*pkt);
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