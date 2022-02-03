#pragma once

#include "SimComponent.h"
#include "RegisterFile.h"
#include "DecodeToExecuteBus.h"
#include "Logger.h"
#include "FetchDecodeBus.h"

namespace ecc
{
	class DecodeStage : public SimComponent
	{
	private:
		FetchToDecodeBus& fetch_bus;
		DecodeToExecuteBus& execute_bus;
		RegisterFile& regs;
		Logger& logger;

	public:
		DecodeStage(SimComponentRegistry& registry, FetchToDecodeBus& _fetch_bus, DecodeToExecuteBus& _execute_bus, RegisterFile& _regs, Logger& _logger)
			: SimComponent(registry, "decode"),
			fetch_bus(_fetch_bus),
			execute_bus(_execute_bus),
			regs(_regs),
			logger(_logger)
		{}

		ecc::ReturnObject run() override;

	private:

		ecc::ReturnObject handle_conditional_jump(uint32_t jmp_mask_const, const FetchToDecodeBusPacket& pkt);
	};

}