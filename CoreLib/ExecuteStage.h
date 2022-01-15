#pragma once

namespace Simulator
{

	class ExecuteStage : public SimComponent
	{
	public:
		ExecuteStage(SimComponentRegistry& registry) : SimComponent(registry, "execute")
		{}

		coro::ReturnObject run() override;
	};
}