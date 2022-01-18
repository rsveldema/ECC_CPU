#pragma once

#include <functional>

#include "MemoryBus.h"

namespace Simulator
{
	/** @brief N-inputs and 1-output.
	* When some data comes back up (a reponse), the multiplexer also de-multiplexes to send the reply to
	* the correct up-stream input.
	*/
	class Multiplexer : public SimComponent
	{
	public:
		using demultiplexer_func_t = std::function<bool(MemoryBus::Packet&)>;

	private:
		MemoryBus& out;

		/** @brief N inputs
		*/
		struct Input {
			MemoryBus* bus;
			demultiplexer_func_t packetResponseShouldBeSentToThisInput;
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
					Task &t = *this;
					co_await t;
				}

				// we should be able to forward the incoming packet to the source
				// in one step, hence no co_await here.
				if (auto pkt = out.try_accept_response())
				{
					bool sent = false;
					for (auto& in : inputs)
					{
						if (in.packetResponseShouldBeSentToThisInput(*pkt))
						{
							sent = true;
							in.bus->send_response(*pkt);
						}
					}
					assert(sent);
				}
				Task& t = *this;
				co_await t;
			}
		}
	};

}