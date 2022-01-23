#include "CoreLib.h"

namespace Simulator
{
	coro::ReturnObject FetchStage::run()
	{
		bool have_outstanding_jmp = false;
		ExecutionMask exec_mask(MachineInfo::ALL_THREADS_EXEC_MASK_INT64);

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

			while (decode_bus.is_busy())
			{
				Task& t = *this;
				co_await t;
			}

			memory_bus.send_read_request_insn(fetch_PC, memory_bus_id);

			while (1)
			{
				if (auto response = memory_bus.try_accept_response())
				{
					auto insn = std::get<MachineInfo::instruction_t>(response->payload);
					//std::cerr << "received response for address " << fetch_PC << std::endl;

					auto opcode = static_cast<MachineInfo::Opcode>(insn & 0xff);
					if (MachineInfo::changesControlFlow(opcode))
					{
						have_outstanding_jmp = true;
					}

					auto PC = fetch_PC;

					fetch_PC += MachineInfo::INSTRUCTION_SIZE;

					FetchToDecodeBus::Packet pkt{ exec_mask, PC, insn };

					this->decode_bus.send(pkt);
					break;
				}
				else
				{
					stats.waitForInsnFetch++;
					Task& t = *this;
					co_await t;
				}
			}

			Task& t = *this;
			co_await t;
		}
	}
}