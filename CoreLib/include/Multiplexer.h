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

		coro::ReturnObject run() override;
	};

}