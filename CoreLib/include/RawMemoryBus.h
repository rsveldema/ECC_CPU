#pragma once

#include <memory>
#include <queue>
#include <optional>
#include <array>
#include <variant>

#include "MachineInfo.h"
#include "VectorValue.h"


namespace ecc
{
	/** simple memory bus that allows sending requests and receiving replies.
	*/
	class RawMemoryBus
	{
	public:
		enum class Type
		{
			read_data,
			write_data,

			read_response,
			write_response
		};

		using payload_t = uint64_t;

		struct Packet
		{
			Type type;
			ecc::BusID source;
			ecc::memory_address_t address;
			payload_t payload;
		};


		std::queue<Packet> request_queue;
		std::queue<Packet> response_queue;


		void send_read_request_data(ecc::memory_address_t address, const ecc::BusID& source)
		{
			Packet pkt{ Type::read_data, source, address };
			send_request(pkt);
		}


		void send_write_request_data(ecc::memory_address_t address,
			const ecc::BusID& source,
			const payload_t& value)
		{
			Packet pkt{ Type::write_data, source, address, value };
			send_request(pkt);
		}

		void send_read_response(const payload_t& value, const ecc::BusID& source)
		{
			ecc::memory_address_t addr = 0;
			Packet pkt{ Type::read_response, source, addr, value };
			send_response(pkt);
		}

		std::optional<Packet> try_accept_request()
		{
			if (!request_queue.empty())
			{
				const Packet f = request_queue.front();
				request_queue.pop();
				return f;
			}
			return std::nullopt;
		}

		std::optional<Packet> try_accept_response()
		{
			if (!response_queue.empty())
			{
				const Packet f = response_queue.front();
				response_queue.pop();
				return f;
			}
			return std::nullopt;
		}

		void send_request(const Packet& pkt)
		{
			assert(!is_busy());
			request_queue.push(pkt);
		}

		void send_response(const Packet& pkt)
		{
			response_queue.push(pkt);
		}

		bool is_busy() const
		{
			return request_queue.size() > 0;
		}

		bool is_reponse_busy() const
		{
			return response_queue.size() > 0;
		}
	};

}