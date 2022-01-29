#include "CoreLib.h"


namespace ecc
{
	ecc::ReturnObject L2DataCache::run()
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

			CONTEXT_SWITCH();
		}
		running = false;
	}
}