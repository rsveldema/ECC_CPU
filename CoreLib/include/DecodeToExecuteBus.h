#pragma once

#include <optional>
#include <variant>

#include "MachineInfo.h"
#include "RegisterFile.h"

namespace Simulator
{

	class DecodeToExecuteBus
	{
	public:
		struct Packet
		{
			MachineInfo::memory_address_t PC;
			MachineInfo::ExecuteStageOpcode opcode;

			std::variant<MachineInfo::RegisterID, VectorValue> value0;
			VectorValue value1;
			VectorValue value2;
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
			const Packet v = queue.front();
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