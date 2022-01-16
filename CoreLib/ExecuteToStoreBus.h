#pragma once

#include <optional>
#include "MachineInfo.h"

namespace Simulator
{

	class ExecuteToStoreBus
	{
	public:
		struct Packet
		{
			uint64_t PC;
			MachineInfo::StorageStageOpcode opcode;
			int64_t dest;
			int64_t value;
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