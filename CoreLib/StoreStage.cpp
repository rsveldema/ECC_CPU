#include "CoreLib.h"


namespace Simulator
{
	coro::ReturnObject StoreStage::run()
	{
		while (1)
		{
			co_await *this;
		}
	}

}