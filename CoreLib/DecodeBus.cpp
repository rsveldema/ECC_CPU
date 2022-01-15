#include "CoreLib.h"


namespace Simulator
{
	coro::ReturnObject DecodeStage::run()
	{
		while (1)
		{
			co_await *this;
		}
	}

}