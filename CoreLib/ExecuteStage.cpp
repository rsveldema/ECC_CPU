#include "CoreLib.h"


namespace Simulator
{

	coro::ReturnObject ExecuteStage::run()
	{
		while (1)
		{
			co_await *this;
		}
	}

}