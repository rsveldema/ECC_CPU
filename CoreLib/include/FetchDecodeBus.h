#pragma once

#include <optional>

namespace Simulator
{

	class FetchToDecodeBus
	{
	public:
		struct Packet
		{
			uint64_t PC;
			uint32_t insn;
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
			auto v = queue.front();
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