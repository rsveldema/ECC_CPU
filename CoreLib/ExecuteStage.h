#pragma once

#include "DecodeToExecuteBus.h"
#include "ExecuteToStoreBus.h"

namespace Simulator
{

	class ExecuteStage : public SimComponent
	{
	private:
		DecodeToExecuteBus& decode_bus;
		ExecuteToStoreBus& store_bus;

	public:
		ExecuteStage(SimComponentRegistry& registry, DecodeToExecuteBus& _decode_bus, ExecuteToStoreBus& _store_bus)
			: SimComponent(registry, "execute"),
			decode_bus(_decode_bus),
			store_bus(_store_bus)
		{}

		coro::ReturnObject run() override;
	};
}