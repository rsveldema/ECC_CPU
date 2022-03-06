#include "CoreLib.h"

namespace ecc
{
		bool oneInputMatches(const BusPacket& pkt, uint32_t NUM_CORES)
        {
            for (uint32_t ix = 0; ix < NUM_CORES; ix++)
			{
				if (pkt.source.core_id == ix)
				{
                    return true;
                }
            }
            return false;
        }


		ReturnObject CoreSelectionMemoryMultiplexer::run(MemoryBus& out, std::array<MemoryBus*, NUM_CORES>& inputs)
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
				
					for (uint32_t ix = 0; ix < NUM_CORES; ix++)
					{
						if (pkt.source.core_id == ix)
						{
							inputs[ix]->send_response(pkt);
						}
					}
					assert(oneInputMatches(pkt, NUM_CORES));
				}

				CONTEXT_SWITCH();
			}
		}

}