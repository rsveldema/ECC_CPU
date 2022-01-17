#include "CoreLib.h"

namespace Simulator
{
	coro::ReturnObject FetchStage::run()
	{
		bool have_outstanding_jmp = false;

		while (1)
		{
			while (have_outstanding_jmp)
			{
				if (auto jmp_retarget = store_bus.try_recv())
				{
					have_outstanding_jmp = false;
					fetch_PC = jmp_retarget->newpc;
					break;
				}
				co_await *this;
			}

			while (decode_bus.is_busy())
			{
				co_await *this;
			}

			unsigned size = MachineInfo::INSTRUCTION_SIZE; // 32 bit fixed width insns
			memory_bus.send_read_request(fetch_PC, memory_bus_id, size);

			while (1)
			{
				if (auto response = memory_bus.try_accept_response())
				{
					auto* ptr = response->payload.data();
					//std::cerr << "received response: " << ptr << std::endl;

					uint32_t insn = *reinterpret_cast<uint32_t*>(ptr);

					auto opcode = static_cast<MachineInfo::Opcode>(insn & 0xff);
					if (MachineInfo::changesControlFlow(opcode))
					{
						have_outstanding_jmp = true;
					}

					auto PC = fetch_PC;

					fetch_PC += MachineInfo::INSTRUCTION_SIZE;

					FetchToDecodeBus::Packet pkt{ PC, insn };

					this->decode_bus.send(pkt);
					break;
				}
				else
				{
					stats.waitForInsnFetch++;
					co_await *this;
				}
			}

			co_await *this;
		}
	}
}