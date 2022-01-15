#include "CoreLib.h"

namespace Simulator
{
	coro::ReturnObject FetchStage::run()
	{
		while (1)
		{
			unsigned size = 4; // 32 bit fixed width insns
			memory_bus.send_read_request(fetch_PC, memory_bus_id, size);

			while (1)
			{
				if (auto response = memory_bus.try_accept_response())
				{
					break;
				}
				else
				{
					co_await *this;
				}
			}


			co_await *this;
		}
	}
}