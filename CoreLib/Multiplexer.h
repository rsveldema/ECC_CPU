#pragma once

#include <functional>

#include "MemoryBus.h"

namespace Simulator
{
	class Multiplexer : public SimComponent
	{
	private:
		MemoryBus& out;

		using demultiplexer_func_t = std::function<bool(MemoryBus::Packet&)>;

		struct Input {
			MemoryBus* bus;
			demultiplexer_func_t selected;
		};

		std::vector<Input> inputs;


	public:
		Multiplexer(SimComponentRegistry& registry, MemoryBus& _out)
			: SimComponent(registry, "multiplexer"),
			out(_out)
		{
		}

		void addInput(MemoryBus* input, demultiplexer_func_t func)
		{
			inputs.push_back(Input{ input, func });
		}

		coro::ReturnObject run() override
		{
			while (1)
			{
				for (auto& in : inputs)
				{
					if (auto pkt = in.bus->try_accept_request())
					{
						out.send_request(*pkt);
					}
					// sending one packet will cost us a cycle (as will testing if an input has a pkt for us to send.
					co_await *this;
				}

				// we should be able to forward the incoming packet to the source
				// in one step, hence no co_await here.
				if (auto pkt = out.try_accept_response())
				{
					for (auto& in : inputs)
					{
						if (in.selected(*pkt))
						{
							in.bus->send_response(*pkt);
						}
					}
				}

				co_await *this;
			}
		}
	};

}