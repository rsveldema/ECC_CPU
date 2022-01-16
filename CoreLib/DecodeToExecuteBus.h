#pragma once

#include <optional>
#include "MachineInfo.h"

namespace Simulator
{

	class DecodeToExecuteBus
	{
	public:
		struct Packet
		{
			uint64_t PC;
			MachineInfo::ExecuteStageOpcode opcode;
			int64_t dest;
			int64_t value1;
			int64_t value2;
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