#pragma once

#include <optional>
#include "MachineInfo.h"

namespace Simulator
{
	class StoreToFetchBus
	{
	public:
		struct Packet
		{
			int64_t newpc;
		};

		void send(const Packet& pkt)
		{
			queue.push(pkt);
		}

		std::optional<Packet> try_recv()
		{
			if (queue.empty())
			{
				return std::nullopt;
			}
			auto v = queue.front();
			queue.pop();
			return v;
		}

		std::queue<Packet> queue;
	};
}