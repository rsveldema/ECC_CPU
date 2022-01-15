#pragma once

#include "SimComponent.h"

namespace Simulator
{

	class StoreStage : public SimComponent
	{
	public:
		StoreStage(SimComponentRegistry& registry) : SimComponent(registry, "storer")
		{}

		coro::ReturnObject run() override;
	};
}
