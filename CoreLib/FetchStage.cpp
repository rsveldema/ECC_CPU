#include "CoreLib.h"

namespace ecc
{
	METHOD_SECTION;

	fetched_instruction_data_t getInsnData(uint64_t value)
	{
		return { static_cast<instruction_t>(value), static_cast<instruction_t>(value >> 32) };
	}

	memory_address_t get_address_to_fetch_from_PC(memory_address_t fetch_PC)
	{
		return fetch_PC & ~7;
	}


	bool changesControlFlow(Opcode op)
	{
		switch (op)
		{
		case INSN_OPCODE_HALT:
		case INSN_OPCODE_JMP_ALWAYS:
		case INSN_OPCODE_JMP_EQUAL:
		case INSN_OPCODE_JMP_NOT_EQUAL:
		case INSN_OPCODE_JMP_GREATER:
		case INSN_OPCODE_JMP_GREATER_EQUAL:
		case INSN_OPCODE_JMP_LOWER:
		case INSN_OPCODE_JMP_LOWER_EQUAL:
		case INSN_OPCODE_LOAD_RESTORE_PC:
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



	template<CoreID core_id>
	ReturnObject FetchStage<core_id>::run(FetchToDecodeBus &decode_bus,
								 StoreToFetchBus &store_bus,
								 MemoryBus &memory_bus,
								 GlobalStats& stats)
	{
		bool have_outstanding_jmp = false;
		memory_address_t fetch_PC = 0;
		execution_mask_t exec_mask = ALL_THREADS_EXEC_MASK_INT64;

		memory_address_t address_cached = 0xffffffffffffffff;
		fetched_instruction_data_t insn_data_cached;

		CONTEXT_SWITCH();


		while (1)
		{
			$display("have outstanding jmp: ", have_outstanding_jmp);

			if (have_outstanding_jmp)
			{
				have_outstanding_jmp = false;

				$display("waiting for store pipline to tell us the cond-jump-address");

				while (1)
				{
					if (store_bus.can_receive)
					{
						StoreToFetchPacket jmp_retarget;
						jmp_retarget = store_bus.recv();
						CONTEXT_SWITCH();
						fetch_PC = jmp_retarget.newpc;
						exec_mask = jmp_retarget.exec_mask;
						CONTEXT_SWITCH();
						break;
					}
					CONTEXT_SWITCH();
				}
			}


			$display("testing cache adress");

			if (address_cached == fetch_PC)
			{
				// nothing to do	
				$display("testing cache adress: already have it cached 1");
			}
			else
			{
				if ((address_cached + sizeof(instruction_t)) == fetch_PC)
				{
					// nothing to do.
					$display("testing cache adress: already have it cached 2");
				}
				else
				{
					$display("requesting memory at address: ", get_address_to_fetch_from_PC(fetch_PC));
					memory_bus.send_read_request_data(get_address_to_fetch_from_PC(fetch_PC), 	
								createBusID(core_id, COMPONENT_TYPE_FETCH));

					while (1)
					{
						if (memory_bus.response_busy)
						{
							BusPacket response = memory_bus.get_response();
					
							CONTEXT_SWITCH();

							$display("response received from caches: ", response.packet_type);
							assert(response.packet_type == bus_read_response);

							address_cached = get_address_to_fetch_from_PC(fetch_PC);				
							insn_data_cached = getInsnData(response.payload);
							CONTEXT_SWITCH();
							break;
						}

						stats.incFetchedInsnWait();
						CONTEXT_SWITCH();
					}
				}
			}

			$display("testing cache adress: retrieving from local cache");


			instruction_t insn = 0;
			if (address_cached == fetch_PC)
			{
				insn = insn_data_cached[0];
			}
			else
			{
				if ((address_cached + sizeof(instruction_t)) == fetch_PC)
				{
					insn = insn_data_cached[1];
				}
				else
				{
					$error("failed to get insn from local fetcher cache");
					assert(false);
				}
			}

			CONTEXT_SWITCH();

			$display("[FETCH] received response for address/insn ", fetch_PC, insn);

			have_outstanding_jmp = changesControlFlow(getOpcode(insn));

			while (decode_bus.is_busy)
			{
				CONTEXT_SWITCH();
			}

			decode_bus.send(create_fetch_decode_packet(exec_mask, fetch_PC, insn));
			fetch_PC += sizeof(instruction_t);

			CONTEXT_SWITCH();
		}
	}
}