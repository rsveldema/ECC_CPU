uint8_t storage[(uint32_t'(END_MEMORY_ADDRESS))];


function void INITIAL_write_to_global_memory(input uint64_t address, input uint8_t data);
begin
begin
	storage[(phys_memory_address_t'(address))] = data;
end
end
endfunction


module DRAM(MemoryBus toCPU);
	reg[32:0] state = 0;
	BusPacket pkt;



	
	
	task run();
	case (state)
		0:
			begin
				state <= 214; // GOTO
				return;
			end
		214:
			begin
				if (!(toCPU.request_busy))
				begin
					state <= 216; // GOTO
					return;
				end
				pkt <= toCPU.accept_request();
				// CONTEXT_SWITCH();
				state <= 217; // GOTO
				return;
			end
		217:
			begin
				state <= 218; // GOTO
				return;
			end
		218:
			begin
				if ((pkt.packet_type == bus_write_data))
				begin
				end
				else
				begin
					state <= 222; // GOTO
					return;
				end
				assert((pkt.address < ( ((uint64_t'($bits(storage)) >> 3)  )  - 8)));
				WRITE_MEMORY_DELAY();
				for (uint64_t i = 0; (i <  ((uint64_t'($bits(bus_packet_payload_t)) >> 3)  ) ); i=(i + 1))
					begin
						storage[(phys_memory_address_t'((pkt.address + i)))] <= (uint8_t'((pkt.payload >> (i * 8))));
					end
				state <= 219; // GOTO
				return;
				state <= 221; // GOTO
				return;
			end
		222:
			begin
				if ((pkt.packet_type == bus_read_data))
				begin
				end
				else
				begin
					state <= 225; // GOTO
					return;
				end
				assert((pkt.address < ( ((uint64_t'($bits(storage)) >> 3)  )  - 8)));
				// CONTEXT_SWITCH();
				state <= 224; // GOTO
				return;
			end
		224:
			begin
				state <= 220; // GOTO
				return;
			end
		220:
			begin
				READ_MEMORY_DELAY();
				toCPU.send_read_response(`PACK8_BYTES(storage[(phys_memory_address_t'((pkt.address + 0)))], storage[(phys_memory_address_t'((pkt.address + 1)))], storage[(phys_memory_address_t'((pkt.address + 2)))], storage[(phys_memory_address_t'((pkt.address + 3)))], storage[(phys_memory_address_t'((pkt.address + 4)))], storage[(phys_memory_address_t'((pkt.address + 5)))], storage[(phys_memory_address_t'((pkt.address + 6)))], storage[(phys_memory_address_t'((pkt.address + 7)))]), pkt.source, pkt.address);
				state <= 219; // GOTO
				return;
				state <= 223; // GOTO
				return;
			end
		225:
			begin
				$display("unrecognized bus command");
				abort();
				state <= 223; // GOTO
				return;
			end
		223:
			begin
				state <= 221; // GOTO
				return;
			end
		221:
			begin
				state <= 219; // GOTO
				return;
			end
		219:
			begin
				state <= 216; // GOTO
				return;
			end
		216:
			begin
				// CONTEXT_SWITCH();
				state <= 226; // GOTO
				return;
			end
		226:
			begin
				state <= 214; // GOTO
				return;
			end
		215:
			begin
			end
	endcase
	endtask
endmodule
