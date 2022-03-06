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

		std::array<MemoryBus*, NUM_CORE_COMP_THAT_CAN_SEND_MSGS> inputs;

	public:
		CoreInternalMemoryMultiplexer(SimComponentRegistry& registry, MemoryBus& _out)
			: SimComponent(registry, "multiplexer"),
			out(_out)
		{
		}

		void addInput(MemoryBus* fetch_input, MemoryBus* other_input)
		{
			inputs[0] = fetch_input;
			inputs[1] = other_input;
		}

		ReturnObject run()
		{
			while (1)
			{
				for (auto& in : inputs)
				{
					if (in->request_busy)
					{
						BusPacket pkt = in->accept_request();

						CONTEXT_SWITCH();

						while (out.request_busy)
						{
							CONTEXT_SWITCH();
						}
						out.send_request(pkt);
					}
					else
					{
						// sending one packet will cost us a cycle (as will testing if an input has a pkt for us to send.
						CONTEXT_SWITCH();
					}
				}

				// we should be able to forward the incoming packet to the source
				// in one step, hence no co_await here.
				if (out.response_busy)
				{
					BusPacket pkt = out.get_response();

					CONTEXT_SWITCH();

					if (pkt.source.within_core_id == CoreComponentID::COMPONENT_TYPE_FETCH)
					{
						inputs[0]->send_response(pkt);
					}
					else
					{
						inputs[1]->send_response(pkt);
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