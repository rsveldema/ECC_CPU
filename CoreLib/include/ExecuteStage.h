#pragma once

#include "DecodeToExecuteBus.h"
#include "ExecuteToStoreBus.h"

namespace ecc
{

	class ExecuteStage : public SimComponent
	{
	private:
		DecodeToExecuteBus& decode_bus;
		ExecuteToStoreBus& store_bus;
		RegisterFile& regs;

	public:
		ExecuteStage(SimComponentRegistry& registry, 
			DecodeToExecuteBus& _decode_bus, 
			ExecuteToStoreBus& _store_bus,
			RegisterFile& _regs)
			: SimComponent(registry, "execute"),
			decode_bus(_decode_bus),
			store_bus(_store_bus),
			regs(_regs)
		{}

		ecc::ReturnObject run() override;
	};
}