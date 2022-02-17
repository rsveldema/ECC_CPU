uint8_t[(END_MEMORY_ADDRESS) - 1:0] storage;


function void write_to_global_memory(input uint64_t address, input uint8_t data);
begin
begin
	storage[address] <= data;
end
end
endfunction


module DRAM();
	reg[32:0] state = 0;
	auto pkt;
	bus_packet_payload_t ret;
	bus_packet_payload_t ret;



	
	
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
							assert((pkt.address >= 0));
							assert((pkt.address < ( ((uint64_t'($bits(storage)) >> 3)  )  - 8)));
							ret <= 0;
							for (int i = 0; (i <  ((uint64_t'($bits(bus_packet_payload_t)) >> 3)  ) ); i=(i + 1))
								begin
									ret <= (ret | ((bus_packet_payload_t'(storage[(pkt.address + i)])) << (i * 8)));
								end
							READ_MEMORY_DELAY();
							toCPU.send_read_response(ret, pkt.source);
							state <= 21; // GOTO
							return;
						end
					bus_write_data:
						begin
							assert((pkt.address >= 0));
							assert((pkt.address < ( ((uint64_t'($bits(storage)) >> 3)  )  - 8)));
							WRITE_MEMORY_DELAY();
							ret <= 0;
							for (int i = 0; (i <  ((uint64_t'($bits(bus_packet_payload_t)) >> 3)  ) ); i=(i + 1))
								begin
									storage[(pkt.address + i)] <= (pkt.payload >> (i * 8));
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
