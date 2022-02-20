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
				state <= 30; // GOTO
				return;
			end
		30:
			begin
				if (!(toCPU.request_busy))
				begin
					state <= 32; // GOTO
					return;
				end
				pkt <= toCPU.accept_request();
				// CONTEXT_SWITCH();
				state <= 33; // GOTO
				return;
			end
		33:
			begin
				state <= 34; // GOTO
				return;
			end
		34:
			begin
				if ((pkt.packet_type == bus_write_data))
				begin
				end
				else
				begin
					state <= 38; // GOTO
					return;
				end
				assert((pkt.address < ( ((uint64_t'($bits(storage)) >> 3)  )  - 8)));
				WRITE_MEMORY_DELAY();
				for (uint64_t i = 0; (i <  ((uint64_t'($bits(bus_packet_payload_t)) >> 3)  ) ); i=(i + 1))
					begin
						storage[(phys_memory_address_t'((pkt.address + i)))] <= (uint8_t'((pkt.payload >> (i * 8))));
					end
				state <= 35; // GOTO
				return;
				state <= 37; // GOTO
				return;
			end
		38:
			begin
				if ((pkt.packet_type == bus_read_data))
				begin
				end
				else
				begin
					state <= 41; // GOTO
					return;
				end
				assert((pkt.address < ( ((uint64_t'($bits(storage)) >> 3)  )  - 8)));
				read_ret <= `PACK8(storage[(phys_memory_address_t'((pkt.address + 0)))], storage[(phys_memory_address_t'((pkt.address + 1)))], storage[(phys_memory_address_t'((pkt.address + 2)))], storage[(phys_memory_address_t'((pkt.address + 3)))], storage[(phys_memory_address_t'((pkt.address + 4)))], storage[(phys_memory_address_t'((pkt.address + 5)))], storage[(phys_memory_address_t'((pkt.address + 6)))], storage[(phys_memory_address_t'((pkt.address + 7)))]);
				// CONTEXT_SWITCH();
				state <= 40; // GOTO
				return;
			end
		40:
			begin
				state <= 36; // GOTO
				return;
			end
		36:
			begin
				READ_MEMORY_DELAY();
				toCPU.send_read_response(read_ret, pkt.source);
				state <= 35; // GOTO
				return;
				state <= 39; // GOTO
				return;
			end
		41:
			begin
				$display("unrecognized bus command");
				abort();
				state <= 39; // GOTO
				return;
			end
		39:
			begin
				state <= 37; // GOTO
				return;
			end
		37:
			begin
				state <= 35; // GOTO
				return;
			end
		35:
			begin
				state <= 32; // GOTO
				return;
			end
		32:
			begin
				// CONTEXT_SWITCH();
				state <= 42; // GOTO
				return;
			end
		42:
			begin
				state <= 30; // GOTO
				return;
			end
		31:
			begin
			end
	endcase
	endtask
endmodule
