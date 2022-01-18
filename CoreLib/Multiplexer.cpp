#include "CoreLib.h"

namespace Simulator
{
    coro::ReturnObject Multiplexer::run()
    {
        while (1)
        {
            for (auto &in : inputs)
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
                for (auto &in : inputs)
                {
                    if (in.packetResponseShouldBeSentToThisInput(*pkt))
                    {
                        sent = true;
                        in.bus->send_response(*pkt);
                    }
                }
                assert(sent);
            }
            Task &t = *this;
            co_await t;
        }
    }
}