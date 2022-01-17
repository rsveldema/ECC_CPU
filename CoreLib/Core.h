#pragma once

namespace Simulator
{
	class Core
	{
	private:
		RegisterFile regs;
		StoreToFetchBus store_fetch_bus;
		FetchToDecodeBus fetch_decode_bus;
		FetchStage fetch;
		DecodeToExecuteBus decode_execute_bus;
		DecodeStage decode;
		ExecuteToStoreBus execute_store_bus;
		ExecuteStage execute;
		StoreStage store;

		// data cache
		Cache L1d;
		MemoryBus core_L1d;

		// instruction cache
		Cache L1i;
		MemoryBus core_L1i;

		Multiplexer multiplexer;

		MemoryBus L1i_multiplexer;
		MemoryBus L1d_multiplexer;

	public:
		Core(SimComponentRegistry& registry, MemoryBus& _memory_bus, MachineInfo::CoreID core_id)
			: regs{},
			fetch(registry, fetch_decode_bus, core_L1i, MemoryBus::createBusID(core_id, MachineInfo::CoreComponentID::FETCH), store_fetch_bus),
			decode(registry, fetch_decode_bus, decode_execute_bus, regs),
			execute(registry, decode_execute_bus, execute_store_bus),
			store(registry, execute_store_bus, core_L1d, regs, MemoryBus::createBusID(core_id, MachineInfo::CoreComponentID::STORE), store_fetch_bus),
			L1i(registry, "L1i", core_L1i, L1i_multiplexer),
			L1d(registry, "L1d", core_L1d, L1d_multiplexer),
			multiplexer(registry, _memory_bus)
		{
			multiplexer.addInput(&L1i_multiplexer, [](const MemoryBus::Packet& p) {
				return p.source.within_core_id == MachineInfo::CoreComponentID::FETCH;
				});
			multiplexer.addInput(&L1d_multiplexer, [](const MemoryBus::Packet& p) {
				return p.source.within_core_id == MachineInfo::CoreComponentID::STORE;
				});
		}

		bool hasHalted()
		{
			return regs.hasHalted();
		}
	};
}
