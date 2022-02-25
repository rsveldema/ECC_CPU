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
	bus_packet_payload_t read_ret;



	
	
	task run();
	case (state)
		0:
			begin
				state <= 128; // GOTO
				return;
			end
		128:
			begin
				if (!(toCPU.request_busy))
				begin
					state <= 130; // GOTO
					return;
				end
				pkt <= toCPU.accept_request();
				// CONTEXT_SWITCH();
				state <= 131; // GOTO
				return;
			end
		131:
			begin
				state <= 132; // GOTO
				return;
			end
		132:
			begin
				if ((pkt.packet_type == bus_write_data))
				begin
				end
				else
				begin
					state <= 136; // GOTO
					return;
				end
				assert((pkt.address < ( ((uint64_t'($bits(storage)) >> 3)  )  - 8)));
				WRITE_MEMORY_DELAY();
				for (uint64_t i = 0; (i <  ((uint64_t'($bits(bus_packet_payload_t)) >> 3)  ) ); i=(i + 1))
					begin
						storage[(phys_memory_address_t'((pkt.address + i)))] <= (uint8_t'((pkt.payload >> (i * 8))));
					end
				state <= 133; // GOTO
				return;
				state <= 135; // GOTO
				return;
			end
		136:
			begin
				if ((pkt.packet_type == bus_read_data))
				begin
				end
				else
				begin
					state <= 139; // GOTO
					return;
				end
				assert((pkt.address < ( ((uint64_t'($bits(storage)) >> 3)  )  - 8)));
				read_ret <= `PACK8(storage[(phys_memory_address_t'((pkt.address + 0)))], storage[(phys_memory_address_t'((pkt.address + 1)))], storage[(phys_memory_address_t'((pkt.address + 2)))], storage[(phys_memory_address_t'((pkt.address + 3)))], storage[(phys_memory_address_t'((pkt.address + 4)))], storage[(phys_memory_address_t'((pkt.address + 5)))], storage[(phys_memory_address_t'((pkt.address + 6)))], storage[(phys_memory_address_t'((pkt.address + 7)))]);
				// CONTEXT_SWITCH();
				state <= 138; // GOTO
				return;
			end
		138:
			begin
				state <= 134; // GOTO
				return;
			end
		134:
			begin
				READ_MEMORY_DELAY();
				toCPU.send_read_response(read_ret, pkt.source);
				state <= 133; // GOTO
				return;
				state <= 137; // GOTO
				return;
			end
		139:
			begin
				$display("unrecognized bus command");
				abort();
				state <= 137; // GOTO
				return;
			end
		137:
			begin
				state <= 135; // GOTO
				return;
			end
		135:
			begin
				state <= 133; // GOTO
				return;
			end
		133:
			begin
				state <= 130; // GOTO
				return;
			end
		130:
			begin
				// CONTEXT_SWITCH();
				state <= 140; // GOTO
				return;
			end
		140:
			begin
				state <= 128; // GOTO
				return;
			end
		129:
			begin
			end
	endcase
	endtask
endmodule
