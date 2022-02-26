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
				state <= 175; // GOTO
				return;
			end
		175:
			begin
				if (!(toCPU.request_busy))
				begin
					state <= 177; // GOTO
					return;
				end
				pkt <= toCPU.accept_request();
				// CONTEXT_SWITCH();
				state <= 178; // GOTO
				return;
			end
		178:
			begin
				state <= 179; // GOTO
				return;
			end
		179:
			begin
				if ((pkt.packet_type == bus_write_data))
				begin
				end
				else
				begin
					state <= 183; // GOTO
					return;
				end
				assert((pkt.address < ( ((uint64_t'($bits(storage)) >> 3)  )  - 8)));
				WRITE_MEMORY_DELAY();
				for (uint64_t i = 0; (i <  ((uint64_t'($bits(bus_packet_payload_t)) >> 3)  ) ); i=(i + 1))
					begin
						storage[(phys_memory_address_t'((pkt.address + i)))] <= (uint8_t'((pkt.payload >> (i * 8))));
					end
				state <= 180; // GOTO
				return;
				state <= 182; // GOTO
				return;
			end
		183:
			begin
				if ((pkt.packet_type == bus_read_data))
				begin
				end
				else
				begin
					state <= 186; // GOTO
					return;
				end
				assert((pkt.address < ( ((uint64_t'($bits(storage)) >> 3)  )  - 8)));
				// CONTEXT_SWITCH();
				state <= 185; // GOTO
				return;
			end
		185:
			begin
				state <= 181; // GOTO
				return;
			end
		181:
			begin
				READ_MEMORY_DELAY();
				toCPU.send_read_response(`PACK8(storage[(phys_memory_address_t'((pkt.address + 0)))], storage[(phys_memory_address_t'((pkt.address + 1)))], storage[(phys_memory_address_t'((pkt.address + 2)))], storage[(phys_memory_address_t'((pkt.address + 3)))], storage[(phys_memory_address_t'((pkt.address + 4)))], storage[(phys_memory_address_t'((pkt.address + 5)))], storage[(phys_memory_address_t'((pkt.address + 6)))], storage[(phys_memory_address_t'((pkt.address + 7)))]), pkt.source);
				state <= 180; // GOTO
				return;
				state <= 184; // GOTO
				return;
			end
		186:
			begin
				$display("unrecognized bus command");
				abort();
				state <= 184; // GOTO
				return;
			end
		184:
			begin
				state <= 182; // GOTO
				return;
			end
		182:
			begin
				state <= 180; // GOTO
				return;
			end
		180:
			begin
				state <= 177; // GOTO
				return;
			end
		177:
			begin
				// CONTEXT_SWITCH();
				state <= 187; // GOTO
				return;
			end
		187:
			begin
				state <= 175; // GOTO
				return;
			end
		176:
			begin
			end
	endcase
	endtask
endmodule
