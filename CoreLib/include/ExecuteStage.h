#pragma once

#include "DecodeToExecuteBus.h"
#include "ExecuteToStoreBus.h"

namespace ecc
{

	template<CoreID core_id>
	class ExecuteStage : public SimComponent
	{
	private:
		DecodeToExecuteBus& _decode_bus;
		ExecuteToStoreBus& _store_bus;
		RegisterFile& _regs;

	public:
		ExecuteStage(SimComponentRegistry& registry, 
			DecodeToExecuteBus& _decode_bus, 
			ExecuteToStoreBus& _store_bus,
			RegisterFile& _regs)
			: SimComponent(registry, "execute"),
			_decode_bus(_decode_bus),
			_store_bus(_store_bus),
			_regs(_regs)
		{}

		ReturnObject run() override
		{
			return run(_decode_bus, _store_bus, _regs);
		}

		ReturnObject run(DecodeToExecuteBus& decode_bus,
					ExecuteToStoreBus& store_bus,
					RegisterFile& regs);
	};

	template class ExecuteStage<CORE_ID_Core0>;
	template class ExecuteStage<CORE_ID_Core1>;
	template class ExecuteStage<CORE_ID_Core2>;
	template class ExecuteStage<CORE_ID_Core3>;
	template class ExecuteStage<CORE_ID_Core4>;
	template class ExecuteStage<CORE_ID_Core5>;
	template class ExecuteStage<CORE_ID_Core6>;
	template class ExecuteStage<CORE_ID_Core7>;
}