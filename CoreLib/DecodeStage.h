#pragma once

#include "SimComponent.h"

namespace Simulator
{
	class DecodeStage : public SimComponent
	{
	private:
		FetchToDecodeBus& fetch_bus;

	public:
		DecodeStage(SimComponentRegistry& registry, FetchToDecodeBus& fetch_bus) : SimComponent(registry, "decode"), fetch_bus(fetch_bus)
		{}

		coro::ReturnObject run() override;
	};

}