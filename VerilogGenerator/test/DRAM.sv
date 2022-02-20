uint8_t storage[END_MEMORY_ADDRESS];


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
	bus_packet_payload_t read_ret;



	
	
	task run();
	case (state)
		0:
			begin
				state <= 78; // GOTO
				return;
			end
		78:
			begin
				if (!(toCPU.request_busy))
				begin
					state <= 80; // GOTO
					return;
				end
				pkt <= toCPU.accept_request();
				// CONTEXT_SWITCH();
				state <= 81; // GOTO
				return;
			end
		81:
			begin
				state <= 82; // GOTO
				return;
			end
		82:
			begin
				if ((pkt.packet_type == bus_write_data))
				begin
				end
				else
				begin
					state <= 86; // GOTO
					return;
				end
				assert((pkt.address < ( ((uint64_t'($bits(storage)) >> 3)  )  - 8)));
				WRITE_MEMORY_DELAY();
				for (uint64_t i = 0; (i <  ((uint64_t'($bits(bus_packet_payload_t)) >> 3)  ) ); i=(i + 1))
					begin
						storage[(phys_memory_address_t'((pkt.address + i)))] <= (uint8_t'((pkt.payload >> (i * 8))));
					end
				state <= 83; // GOTO
				return;
				state <= 85; // GOTO
				return;
			end
		86:
			begin
				if ((pkt.packet_type == bus_read_data))
				begin
				end
				else
				begin
					state <= 89; // GOTO
					return;
				end
				assert((pkt.address < ( ((uint64_t'($bits(storage)) >> 3)  )  - 8)));
				read_ret <= `PACK8(storage[(phys_memory_address_t'((pkt.address + 0)))], storage[(phys_memory_address_t'((pkt.address + 1)))], storage[(phys_memory_address_t'((pkt.address + 2)))], storage[(phys_memory_address_t'((pkt.address + 3)))], storage[(phys_memory_address_t'((pkt.address + 4)))], storage[(phys_memory_address_t'((pkt.address + 5)))], storage[(phys_memory_address_t'((pkt.address + 6)))], storage[(phys_memory_address_t'((pkt.address + 7)))]);
				// CONTEXT_SWITCH();
				state <= 88; // GOTO
				return;
			end
		88:
			begin
				state <= 84; // GOTO
				return;
			end
		84:
			begin
				READ_MEMORY_DELAY();
				toCPU.send_read_response(read_ret, pkt.source);
				state <= 83; // GOTO
				return;
				state <= 87; // GOTO
				return;
			end
		89:
			begin
				$display("unrecognized bus command");
				abort();
				state <= 87; // GOTO
				return;
			end
		87:
			begin
				state <= 85; // GOTO
				return;
			end
		85:
			begin
				state <= 83; // GOTO
				return;
			end
		83:
			begin
				state <= 80; // GOTO
				return;
			end
		80:
			begin
				// CONTEXT_SWITCH();
				state <= 90; // GOTO
				return;
			end
		90:
			begin
				state <= 78; // GOTO
				return;
			end
		79:
			begin
			end
	endcase
	endtask
endmodule
