#pragma once

#include <optional>

#include "MachineInfo.h"
#include "ExecutionMask.h"

namespace ecc
{
	class StoreToFetchBus
	{
	public:
		struct Packet
		{
			ExecutionMask exec_mask;
			ecc::memory_address_t newpc;
		};

		void send(const Packet& pkt)
		{
			assert(!is_busy());
			queue.push(pkt);
		}

		std::optional<Packet> try_recv()
		{
			if (queue.empty())
			{
				return std::nullopt;
			}
			Packet v = queue.front();
			queue.pop();
			return v;
		}

		bool is_busy() const
		{
			return queue.size() > 0;
		}

		std::queue<Packet> queue;
	};
}