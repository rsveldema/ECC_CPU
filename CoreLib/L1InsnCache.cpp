#include "CoreLib.h"


namespace ecc
{
	struct L1InsnCacheLine {
		memory_address_t address;
		int64_t payload;
	};

	static constexpr uint32_t NUM_CACHE_LINES_L1_INSN_CACHE = 128;


	METHOD_SECTION;

	uint32_t get_l1_insn_cache_index(memory_address_t address)
	{
		assert((address & (8-1)) == 0);
		return (static_cast<uint32_t>(address) >> 3) & (NUM_CACHE_LINES_L1_INSN_CACHE - 1);
	}


	template <CoreID core_id>
	ReturnObject L1InsnCache<core_id>::run(MemoryBus& toCPU, MemoryBus& toMemory, GlobalStats& stats)	
	{
		std::array<L1InsnCacheLine, NUM_CACHE_LINES_L1_INSN_CACHE> L1InsnCacheLines;
		for (uint32_t i=0;i<NUM_CACHE_LINES_L1_INSN_CACHE;i++)
		{
			L1InsnCacheLines[i].address = 100000000;
		}

		CONTEXT_SWITCH();

		while (1)
		{
			if (toCPU.request_busy)
			{
				BusPacket cpu_pkt = toCPU.accept_request();

				CONTEXT_SWITCH();

				if ((L1InsnCacheLines[(get_l1_insn_cache_index(cpu_pkt.address))].address) == (cpu_pkt.address))
				{
					stats.incnumL1InsnCacheHits();
					while (toCPU.response_busy)
					{
						CONTEXT_SWITCH();
					}
					toCPU.send_read_response(L1InsnCacheLines[get_l1_insn_cache_index(cpu_pkt.address)].payload, cpu_pkt.source, cpu_pkt.address);
				}
				else
				{
					while (toMemory.request_busy)
					{
						CONTEXT_SWITCH();
					}				

					toMemory.send_request(cpu_pkt);
				}
			}

			if (toMemory.response_busy)
			{
				BusPacket response_pkt = toMemory.get_response();
				CONTEXT_SWITCH();

				$display("storing into cache: address: ", static_cast<uint32_t>(response_pkt.address), get_l1_insn_cache_index(response_pkt.address), static_cast<uint32_t>(response_pkt.payload));

				L1InsnCacheLines[get_l1_insn_cache_index(response_pkt.address)] = { response_pkt.address, static_cast<int64_t>(response_pkt.payload) };

				while (toCPU.response_busy)
				{
					CONTEXT_SWITCH();
				}
				toCPU.send_response(response_pkt);
			}

			CONTEXT_SWITCH();
		}
	}
}