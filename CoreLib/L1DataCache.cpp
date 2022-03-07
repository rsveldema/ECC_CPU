#include "CoreLib.h"


namespace ecc
{
	struct L1DataCacheLine {
		memory_address_t address;
		VectorValue payload;
	};

	static constexpr uint32_t NUM_CACHE_LINES_L1_DATA_CACHE = 128;


	METHOD_SECTION;

	uint32_t get_l1_data_cache_index(memory_address_t address)
	{
		assert((address & (8-1)) == 0);
		return (static_cast<uint32_t>(address) >> 3) & (NUM_CACHE_LINES_L1_DATA_CACHE - 1);
	}

	template <CoreID core_id>
	ReturnObject L1DataCache<core_id>::run()
	{
		while (1)
		{
			if (! toMemory.request_busy)
			{
				if (toCPU.request_busy)
				{
					auto pkt = toCPU.accept_request();
					toMemory.send_request(pkt);
				}
			}

			if (!toCPU.response_busy)
			{
				if (toMemory.response_busy)
				{
					auto pkt = toMemory.get_response();
					toCPU.send_response(pkt);
				}
			}

			CONTEXT_SWITCH();
		}
	}
}