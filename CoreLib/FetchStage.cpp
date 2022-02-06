#include "CoreLib.h"

namespace ecc
{
	union int64_to_insn_data 
	{
		uint64_t value;
		fetched_instruction_data_t data;
	};

	METHOD_SECTION;

	fetched_instruction_data_t getInsnData(uint64_t value)
	{
		int64_to_insn_data tmp;
		tmp.value = value;
		return tmp.data;
	}

	bool changesControlFlow(Opcode op)
	{
		switch (op)
		{
		case Opcode::HALT:
		case Opcode::JMP_ALWAYS:
		case Opcode::JMP_EQUAL:
		case Opcode::JMP_NOT_EQUAL:
		case Opcode::JMP_GREATER:
		case Opcode::JMP_GREATER_EQUAL:
		case Opcode::JMP_LOWER:
		case Opcode::JMP_LOWER_EQUAL:
		case Opcode::LOAD_RESTORE_PC:
		{
			return true;
		}
		default:
		{
			return false;
		}
		}
		return false;
	}

	ReturnObject FetchStage::run(FetchToDecodeBus &decode_bus,
								 StoreToFetchBus &store_bus,
								 MemoryBus &memory_bus)
	{
		bool have_outstanding_jmp = false;
		memory_address_t fetch_PC = 0;
		execution_mask_t exec_mask(ALL_THREADS_EXEC_MASK_INT64);

		memory_address_t address_cached = 0xffffffff;
		fetched_instruction_data_t fetched_cached{};

		while (1)
		{
			if (have_outstanding_jmp)
			{
				have_outstanding_jmp = false;

				while (1)
				{
					if (store_bus.can_receive)
					{
						StoreToFetchPacket jmp_retarget = store_bus.recv();
						fetch_PC = jmp_retarget.newpc;
						exec_mask = jmp_retarget.exec_mask;
						break;
					}
					CONTEXT_SWITCH();
				}
			}

			if (address_cached == fetch_PC)
			{
				// nothing to do
			}
			else
			{
				if ((address_cached + sizeof(instruction_t)) == fetch_PC)
				{
					// nothing to do.
				}
				else
				{
					const memory_address_t address_fetched = fetch_PC & ~7;

					memory_bus.send_read_request_data(address_fetched, memory_bus_id);

					while (1)
					{
						if (memory_bus.response_busy)
						{
							BusPacket response = memory_bus.get_response();
							assert(response.packet_type == BusPacketType::read_response);

							address_cached = address_fetched;							
							fetched_cached = getInsnData(response.payload);
							break;
						}

						stats.incFetchedInsns();
						CONTEXT_SWITCH();
					}
				}
			}

			instruction_t insn = 0;
			if (address_cached == fetch_PC)
			{
				insn = fetched_cached[0];
			}
			else
			{
				if ((address_cached + sizeof(instruction_t)) == fetch_PC)
				{
					insn = fetched_cached[1];
				}
				else
				{
					logger.error("failed to get insn from local fetcher cache");
					abort();
				}
			}

			// logger.debug("[FETCH] received response for address " + std::to_string(fetch_PC));

			Opcode opcode = static_cast<Opcode>(insn & 0xff);
			if (changesControlFlow(opcode))
			{
				have_outstanding_jmp = true;
			}

			while (decode_bus.is_busy)
			{
				CONTEXT_SWITCH();
			}

			const memory_address_t PC = fetch_PC;
			fetch_PC += sizeof(instruction_t);
			FetchToDecodeBusPacket pkt{exec_mask, PC, insn};
			decode_bus.send(pkt);

			CONTEXT_SWITCH();
		}
	}
}