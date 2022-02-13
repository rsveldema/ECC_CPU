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
								 MemoryBus &memory_bus)
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
					const memory_address_t address_fetched = fetch_PC & ~7;


					$display("requesting memory at address: ", address_fetched);


					memory_bus.send_read_request_data(address_fetched, 	
								createBusID(core_id, COMPONENT_TYPE_FETCH));

					while (1)
					{
						if (memory_bus.response_busy)
						{
							BusPacket response = memory_bus.get_response();
					
							$display("response received from caches: ", response.packet_type);

							CONTEXT_SWITCH();
							assert(response.packet_type == bus_read_response);

							address_cached = address_fetched;							
							insn_data_cached = getInsnData(response.payload);
							CONTEXT_SWITCH();
							break;
						}

						incFetchedInsnWait();
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

			// debug("[FETCH] received response for address " + std::to_string(fetch_PC));

			have_outstanding_jmp = changesControlFlow(getOpcode(insn));

			while (decode_bus.is_busy)
			{
				CONTEXT_SWITCH();
			}

			const memory_address_t old_PC = fetch_PC;
			fetch_PC += sizeof(instruction_t);
			FetchToDecodeBusPacket pkt = create_fetch_decode_packet(exec_mask, old_PC, insn);
			decode_bus.send(pkt);

			CONTEXT_SWITCH();
		}
	}
}