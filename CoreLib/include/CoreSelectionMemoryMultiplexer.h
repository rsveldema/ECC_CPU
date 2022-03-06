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
	class CoreSelectionMemoryMultiplexer : public SimComponent
	{
	public:
		using PacketType = BusPacket;

		static constexpr uint32_t NUM_CORES = 1;

	private:
		MemoryBus& out;

		/** @brief N inputs
		*/
		std::vector<MemoryBus*> inputs;


	public:		
	
		void addInput(MemoryBus* input)
		{
			inputs.push_back(input);
		}

		CoreSelectionMemoryMultiplexer(SimComponentRegistry& registry, MemoryBus& _out)
			: SimComponent(registry, "multiplexer"),
			out(_out)
		{
		}


		ReturnObject run()
		{
			while (1)
			{
				for (uint32_t ix = 0; ix < NUM_CORES; ix++)
				{
					if (inputs[ix]->request_busy)
					{
						BusPacket pkt = inputs[ix]->accept_request();

						CONTEXT_SWITCH();

						while (out.request_busy)
						{
							CONTEXT_SWITCH();
						}
						out.send_request(pkt);
					}
					// sending one packet will cost us a cycle (as will testing if an input has a pkt for us to send.
					CONTEXT_SWITCH();
				}

				// we should be able to forward the incoming packet to the source
				// in one step, hence no co_await here.
				if (out.response_busy)
				{
					BusPacket pkt = out.get_response();

					CONTEXT_SWITCH();
				
					bool sent = false;
					for (uint32_t ix = 0; ix < NUM_CORES; ix++)
					{
						if (pkt.source.core_id == ix)
						{
							sent = true;
							inputs[ix]->send_response(pkt);
						}
					}
					assert(sent);
				}

				CONTEXT_SWITCH();
			}
		}
	};

}