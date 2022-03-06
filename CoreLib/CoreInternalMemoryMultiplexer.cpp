#include "CoreLib.h"

namespace ecc
{
	METHOD_SECTION;

 		ReturnObject CoreInternalMemoryMultiplexer::run(MemoryBus& out, MemoryBus& fetch_input, MemoryBus& other_input)
		{
			while (1)
			{
				if (fetch_input.request_busy)
				{
					BusPacket fetch_pkt = fetch_input.accept_request();

					CONTEXT_SWITCH();

					while (out.request_busy)
					{
						CONTEXT_SWITCH();
					}
					out.send_request(fetch_pkt);
				}

				if (other_input.request_busy)
				{
					BusPacket other_pkt = other_input.accept_request();

					CONTEXT_SWITCH();

					while (out.request_busy)
					{
						CONTEXT_SWITCH();
					}
					out.send_request(other_pkt);
				}

				// we should be able to forward the incoming packet to the source
				// in one step, hence no co_await here.
				if (out.response_busy)
				{
					BusPacket out_pkt = out.get_response();

					CONTEXT_SWITCH();

					if (out_pkt.source.within_core_id == COMPONENT_TYPE_FETCH)
					{
						fetch_input.send_response(out_pkt);
					}
					else
					{
						other_input.send_response(out_pkt);
					}
				}
				else
				{
					CONTEXT_SWITCH();
				}
			}
		}
   
}