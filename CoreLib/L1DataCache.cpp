#include "CoreLib.h"


namespace ecc
{
	ecc::ReturnObject L1DataCache::run()
	{
		running = true;
		while (1)
		{
			if (! toMemory.is_busy())
			{
				if (auto pkt = toCPU.try_accept_request())
				{
					toMemory.send_request(*pkt);
				}
			}

			if (!toCPU.is_reponse_busy())
			{
				if (toMemory.have_response())
				{
					auto pkt = toMemory.get_response();
					toCPU.send_response(pkt);
				}
			}

			CONTEXT_SWITCH();
		}
		running = false;
	}
}