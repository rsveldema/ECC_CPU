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

		ReturnObject run(MemoryBus& out, MemoryBus& fetch_input, MemoryBus& other_input)
		{
			while (1)
			{
				if (fetch_input.request_busy)
				{
					BusPacket pkt = fetch_input.accept_request();

					CONTEXT_SWITCH();

					while (out.request_busy)
					{
						CONTEXT_SWITCH();
					}
					out.send_request(pkt);
				}

				if (other_input.request_busy)
				{
					BusPacket pkt = other_input.accept_request();

					CONTEXT_SWITCH();

					while (out.request_busy)
					{
						CONTEXT_SWITCH();
					}
					out.send_request(pkt);
				}

				// we should be able to forward the incoming packet to the source
				// in one step, hence no co_await here.
				if (out.response_busy)
				{
					BusPacket pkt = out.get_response();

					CONTEXT_SWITCH();

					if (pkt.source.within_core_id == CoreComponentID::COMPONENT_TYPE_FETCH)
					{
						fetch_input.send_response(pkt);
					}
					else
					{
						other_input.send_response(pkt);
					}
				}
				else
				{
					CONTEXT_SWITCH();
				}
			}
		}
	};

}