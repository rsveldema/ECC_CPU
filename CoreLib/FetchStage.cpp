#include "CoreLib.h"

namespace Simulator
{
	coro::ReturnObject FetchStage::run()
	{
		bool have_outstanding_jmp = false;
		MachineInfo::memory_address_t fetch_PC = 0;
		ExecutionMask exec_mask(MachineInfo::ALL_THREADS_EXEC_MASK_INT64);

		MachineInfo::memory_address_t address_cached = 0xffffffff;
		MachineInfo::fetched_instruction_data_t fetched_cached;

		while (1)
		{
			while (have_outstanding_jmp)
			{
				if (auto jmp_retarget = store_bus.try_recv())
				{
					have_outstanding_jmp = false;
					fetch_PC = jmp_retarget->newpc;
					exec_mask = jmp_retarget->exec_mask;
					break;
				}
				Task& t = *this;
				co_await t;
			}



			if (address_cached == fetch_PC)
			{
				// nothing to do
			}
			else if ((address_cached + sizeof(MachineInfo::instruction_t)) == fetch_PC)
			{
				// nothing to do.
			}
			else
			{
				const auto address_fetched = fetch_PC & ~7;

				memory_bus.send_read_request_insn(address_fetched, memory_bus_id);

				while (1)
				{
					if (auto response = memory_bus.try_accept_response())
					{
						assert(response->type == InsnCacheMemoryBus::Type::read_response);

						address_cached = address_fetched;
						fetched_cached = std::get<MachineInfo::fetched_instruction_data_t>(response->payload);
						break;
					}

					stats.waitForInsnFetch++;
					Task& t = *this;
					co_await t;
				}
			}

			MachineInfo::instruction_t insn = 0;
			if (address_cached == fetch_PC)
			{
				insn = fetched_cached[0];
			}
			else if ((address_cached + MachineInfo::INSTRUCTION_SIZE) == fetch_PC)
			{
				insn = fetched_cached[1];
			}
			else
			{
				std::cerr << "failed to get insn from local fetcher cache" << std::endl;
				abort();
			}

			//logger.debug("[FETCH] received response for address " + std::to_string(fetch_PC));

			auto opcode = static_cast<MachineInfo::Opcode>(insn & 0xff);
			if (MachineInfo::changesControlFlow(opcode))
			{
				have_outstanding_jmp = true;
			}

			while (decode_bus.is_busy())
			{
				Task& t = *this;
				co_await t;
			}

			auto PC = fetch_PC;
			fetch_PC += MachineInfo::INSTRUCTION_SIZE;
			FetchToDecodeBus::Packet pkt{ exec_mask, PC, insn };
			this->decode_bus.send(pkt);


			Task& t = *this;
			co_await t;
		}
	}
}