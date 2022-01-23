#pragma once

#include <optional>
#include "MachineInfo.h"
#include "ExecutionMask.h"

namespace Simulator
{

	class ExecuteToStoreBus
	{
	public:
		struct Packet
		{
			ExecutionMask exec_mask;
			MachineInfo::memory_address_t PC;
			MachineInfo::StorageStageOpcode opcode;

			// in case its an address / register:
			std::variant<MachineInfo::memory_address_t, VectorValue, MachineInfo::RegisterID> dest;
			std::variant<MachineInfo::memory_address_t, VectorValue, MachineInfo::RegisterID> src;

			bool is_store_to_pc = false;

			ExecutionMask execution_flags_true;
			ExecutionMask execution_flags_false;
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