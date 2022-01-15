#include "CoreLib.h"

namespace Simulator
{
	coro::ReturnObject FetchStage::run()
	{
		while (1)
		{
			unsigned size = MachineInfo::INSTRUCTION_SIZE; // 32 bit fixed width insns
			memory_bus.send_read_request(fetch_PC, memory_bus_id, size);

			while (1)
			{
				if (auto response = memory_bus.try_accept_response())
				{
					auto* ptr = response->payload.data();
					std::cerr << "received response: " << ptr << std::endl;

					uint32_t insn = *reinterpret_cast<uint32_t*>(ptr);

					auto PC = fetch_PC;

					fetch_PC += MachineInfo::INSTRUCTION_SIZE;

					FetchToDecodeBus::Packet pkt{ PC, insn };

					this->decode_bus.send(pkt);
					break;
				}
				else
				{
					co_await *this;
				}
			}


			co_await *this;
		}
	}
}