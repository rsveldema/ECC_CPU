#pragma once

#include <memory>
#include <queue>
#include <optional>
#include <array>

#include "MachineInfo.h"

namespace Simulator
{

	class MemoryBus
	{
	public:
		struct BusID
		{
			MachineInfo::CoreID core_id;
			MachineInfo::CoreComponentID within_core_id;
		};

		using memory_address_t = uint64_t;

		enum class Type
		{
			read,
			write,
			read_response,
			write_response
		};

		using payload_t = std::array<byte, 16>;

		struct Packet
		{
			Type type;
			BusID source;
			memory_address_t address;
			unsigned size;
			payload_t payload;
		};

		std::queue<Packet> request_queue;
		std::queue<Packet> response_queue;

		static BusID createBusID(MachineInfo::CoreID core_id, MachineInfo::CoreComponentID within_id)
		{
			return { core_id, within_id };
		}

		void send_read_request(memory_address_t address, const BusID& source, unsigned size)
		{
			Packet pkt{ Type::read, source, address, size };
			send_request(pkt);
		}

		void send_write_request(memory_address_t address,
			const BusID& source,
			unsigned size,
			uint64_t data)
		{
			Packet pkt{ Type::write, source, address, size };
			*reinterpret_cast<uint64_t*>(&pkt.payload) = data;
			send_request(pkt);
		}

		void send_read_response(const payload_t& value, const BusID& source, unsigned size)
		{
			Packet pkt{ Type::read_response, source, 0, size, value };
			send_response(pkt);
		}

		std::optional<Packet> try_accept_request()
		{
			if (!request_queue.empty())
			{
				auto f = request_queue.front();
				request_queue.pop();
				return f;
			}
			return std::nullopt;
		}

		std::optional<Packet> try_accept_response()
		{
			if (!response_queue.empty())
			{
				auto f = response_queue.front();
				response_queue.pop();
				return f;
			}
			return std::nullopt;
		}

		void send_request(const Packet& pkt)
		{
			request_queue.push(pkt);
		}

		void send_response(const Packet& pkt)
		{
			response_queue.push(pkt);
		}
	};

}