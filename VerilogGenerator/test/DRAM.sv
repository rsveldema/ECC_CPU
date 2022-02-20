uint8_t storage[END_MEMORY_ADDRESS];


function void write_to_global_memory(input uint64_t address, input uint8_t data);
begin
begin
	storage[(phys_memory_address_t'(address))] <= data;
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
				state <= 20; // GOTO
				return;
			end
		20:
			begin
				if (!(toCPU.request_busy))
				begin
					state <= 22; // GOTO
					return;
				end
				pkt <= toCPU.accept_request();
				// CONTEXT_SWITCH();
				case (pkt.packet_type)
					bus_read_data:
						begin
							assert((pkt.address < ( ((uint64_t'($bits(storage)) >> 3)  )  - 8)));
							read_ret <= `PACK8(storage[(phys_memory_address_t'((pkt.address + 0)))], storage[(phys_memory_address_t'((pkt.address + 1)))], storage[(phys_memory_address_t'((pkt.address + 2)))], storage[(phys_memory_address_t'((pkt.address + 3)))], storage[(phys_memory_address_t'((pkt.address + 4)))], storage[(phys_memory_address_t'((pkt.address + 5)))], storage[(phys_memory_address_t'((pkt.address + 6)))], storage[(phys_memory_address_t'((pkt.address + 7)))]);
							READ_MEMORY_DELAY();
							toCPU.send_read_response(read_ret, pkt.source);
							state <= 21; // GOTO
							return;
						end
					bus_write_data:
						begin
							assert((pkt.address < ( ((uint64_t'($bits(storage)) >> 3)  )  - 8)));
							WRITE_MEMORY_DELAY();
							for (uint64_t i = 0; (i <  ((uint64_t'($bits(bus_packet_payload_t)) >> 3)  ) ); i=(i + 1))
								begin
									storage[(phys_memory_address_t'((pkt.address + i)))] <= (uint8_t'((pkt.payload >> (i * 8))));
								end
							state <= 21; // GOTO
							return;
						end
					default:
						begin
							$display("unrecognized bus command");
							abort();
						end
				endcase
				state <= 22; // GOTO
				return;
			end
		22:
			begin
				// CONTEXT_SWITCH();
				state <= 20; // GOTO
				return;
			end
		21:
			begin
			end
	endcase
	endtask
endmodule
