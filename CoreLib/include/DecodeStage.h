#pragma once

#include "SimComponent.h"
#include "RegisterFile.h"
#include "DecodeToExecuteBus.h"
#include "Logger.h"
#include "FetchToDecodeBus.h"

namespace ecc
{	

	template<CoreID core_id>
	class DecodeStage : public SimComponent
	{
	private:
		FetchToDecodeBus& _fetch_bus;
		DecodeToExecuteBus& _execute_bus;
		RegisterFile& _regs;

	public:
		DecodeStage(SimComponentRegistry& registry, 
				FetchToDecodeBus& _fetch_bus, 
				DecodeToExecuteBus& _execute_bus, 
				RegisterFile& _regs)
			: SimComponent(registry, "decode"),
			_fetch_bus(_fetch_bus),
			_execute_bus(_execute_bus),
			_regs(_regs)
		{}

		ReturnObject run() override
		{
			return run(_fetch_bus, _execute_bus, _regs);
		}

		ReturnObject run(FetchToDecodeBus& fetch_bus,
						DecodeToExecuteBus& execute_bus,
						RegisterFile& regs);		

	private:

		ecc::ReturnObject handle_conditional_jump(uint32_t jmp_mask_const, const FetchToDecodeBusPacket& pkt);
	};

	template class DecodeStage<CORE_ID_Core0>;
	template class DecodeStage<CORE_ID_Core1>;
	template class DecodeStage<CORE_ID_Core2>;
	template class DecodeStage<CORE_ID_Core3>;
	template class DecodeStage<CORE_ID_Core4>;
	template class DecodeStage<CORE_ID_Core5>;
	template class DecodeStage<CORE_ID_Core6>;
	template class DecodeStage<CORE_ID_Core7>;


}