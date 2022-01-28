#include "CoreLib.h"

namespace ecc
{
	ReturnObject FetchStage::run()
	{
		bool have_outstanding_jmp = false;
		memory_address_t fetch_PC = 0;
		ExecutionMask exec_mask(ALL_THREADS_EXEC_MASK_INT64);

		memory_address_t address_cached = 0xffffffff;
		fetched_instruction_data_t fetched_cached{};

		while (1)
		{
			while (have_outstanding_jmp)
			{
				auto jmp_retarget = store_bus.try_recv();
				if (jmp_retarget)
				{
					have_outstanding_jmp = false;
					fetch_PC = jmp_retarget->newpc;
					exec_mask = jmp_retarget->exec_mask;
					break;
				}
				CONTEXT_SWITCH();
			}

			if (address_cached == fetch_PC)
			{
				// nothing to do
			}
			else
			{
				if ((address_cached + sizeof(instruction_t)) == fetch_PC)
				{
					// nothing to do.
				}
				else
				{
					const auto address_fetched = fetch_PC & ~7;

					memory_bus.send_read_request_insn(address_fetched, memory_bus_id);

					while (1)
					{
						auto response = memory_bus.try_accept_response();
						if (response)
						{
							assert(response->type == InsnCacheMemoryBus::Type::read_response);

							address_cached = address_fetched;
							fetched_cached = response->getInsnData();
							break;
						}

						stats.incFetchedInsns();
						CONTEXT_SWITCH();
					}
				}
			}

			instruction_t insn = 0;
			if (address_cached == fetch_PC)
			{
				insn = fetched_cached[0];
			}
			else
			{
				if ((address_cached + INSTRUCTION_SIZE) == fetch_PC)
				{
					insn = fetched_cached[1];
				}
				else
				{
					logger.error("failed to get insn from local fetcher cache");
					abort();
				}
			}

			//logger.debug("[FETCH] received response for address " + std::to_string(fetch_PC));

			auto opcode = static_cast<Opcode>(insn & 0xff);
			if (changesControlFlow(opcode))
			{
				have_outstanding_jmp = true;
			}

			while (decode_bus.is_busy())
			{
				CONTEXT_SWITCH();
			}

			const auto PC = fetch_PC;
			fetch_PC += INSTRUCTION_SIZE;
			FetchToDecodeBus::Packet pkt{exec_mask, PC, insn};
			decode_bus.send(pkt);

			CONTEXT_SWITCH();
		}
	}
}