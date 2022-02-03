#include "CoreLib.h"


namespace ecc
{
	ecc::ReturnObject L3Cache::run()
	{
		running = true;
		while (1)
		{
			if (!toMemory.request_busy)
			{
				if (toCPU.request_busy)
				{
					auto pkt = toCPU.accept_request();
					toMemory.send_request(pkt);
				}
			}

			if (toMemory.response_busy)
			{
				auto pkt = toMemory.get_response();			
				toCPU.send_response(pkt);
			}

			CONTEXT_SWITCH();
		}
		running = false;
	}
}