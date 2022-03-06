#pragma once

#include <functional>
#include "MemoryBus.h"
#include "SimComponent.h"


namespace ecc
{
	/** @brief N-inputs and 1-output.
	* When some data comes back up (a reponse), the multiplexer also de-multiplexes to send the reply to
	* the correct up-stream input.
	*/
	class CoreInternalMemoryMultiplexer : public SimComponent
	{
	public:
		using PacketType = BusPacket;

	private:

		static constexpr uint32_t NUM_CORE_COMP_THAT_CAN_SEND_MSGS = 2;

		MemoryBus& out;
		MemoryBus& fetch_input;
		MemoryBus& other_input;

	public:
		CoreInternalMemoryMultiplexer(SimComponentRegistry& registry, MemoryBus& _out, MemoryBus& fetch_input, MemoryBus& other_input)
			: SimComponent(registry, "multiplexer"),
			out(_out), fetch_input(fetch_input), other_input(other_input)
		{
		}


		ReturnObject run() override {
			return run(out, fetch_input, other_input);
		}

		ReturnObject run(MemoryBus& out, MemoryBus& fetch_input, MemoryBus& other_input);

	};

}