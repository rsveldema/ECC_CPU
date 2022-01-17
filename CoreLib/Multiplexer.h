#pragma once

namespace Simulator
{
	class Multiplexer : public SimComponent
	{
	public:
		Multiplexer(SimComponentRegistry& registry) : SimComponent(registry, "multiplexer")
		{
		}

		coro::ReturnObject run() override
		{
			while (1)
			{
				co_await *this;
			}
		}
	};

}