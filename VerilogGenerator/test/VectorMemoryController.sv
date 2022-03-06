

module VectorMemoryController(VecMemoryBus toCPU, MemoryBus toMemory);
	parameter CoreID core_id;
	reg[32:0] state = 0;
	uint32_t i;
	VecBusPacket pkt;
	BusPacket mem_pkt;



	
	
	task run();
	case (state)
		0:
			begin
				i <= 0;
				state <= 227; // GOTO
				return;
			end
		227:
			begin
				if (!(toCPU.request_busy))
				begin
					state <= 229; // GOTO
					return;
				end
				pkt <= toCPU.accept_request();
				// CONTEXT_SWITCH();
				state <= 230; // GOTO
				return;
			end
		230:
			begin
				assert(areAllValidMemoryAddresses(pkt.address));
				if ((pkt.packet_type == VEC_BUS_PKT_TYPE_read_vec64))
				begin
				end
				else
				begin
					state <= 243; // GOTO
					return;
				end
				$display("[VEC-CTRL] going to request read memory");
				pkt.packet_type <= VEC_BUS_PKT_TYPE_read_response_vec64;
				i <= 0;
				state <= 232; // GOTO
				return;
			end
		232:
			begin
				state <= 234; // GOTO
				return;
			end
		234:
			begin
				// CONTEXT_SWITCH();
				state <= 236; // GOTO
				return;
			end
		236:
			begin
				if (toMemory.request_busy)
				begin
					state <= 234; // GOTO
					return;
				end
				state <= 235; // GOTO
				return;
			end
		235:
			begin
				$display("[VEC-CTRL] going to request read memory: target idle, req addr ", pkt.address.data[i]);
				toMemory.send_read_request_data(pkt.address.data[i], pkt.source);
				state <= 237; // GOTO
				return;
			end
		237:
			begin
				if (!(toMemory.response_busy))
				begin
					state <= 239; // GOTO
					return;
				end
				mem_pkt <= toMemory.get_response();
				// CONTEXT_SWITCH();
				state <= 240; // GOTO
				return;
			end
		240:
			begin
				assert((mem_pkt.source.core_id == pkt.source.core_id));
				assert((mem_pkt.source.within_core_id == pkt.source.within_core_id));
				$display("[VEC-CTRL] read value ", mem_pkt.payload, " for address ", pkt.address.data[i]);
				pkt.payload.data[i] <= mem_pkt.payload;
				state <= 238; // GOTO
				return;
			end
		239:
			begin
				// CONTEXT_SWITCH();
				state <= 241; // GOTO
				return;
			end
		241:
			begin
				state <= 237; // GOTO
				return;
			end
		238:
			begin
				i <= (i + 1);
				// CONTEXT_SWITCH();
				state <= 242; // GOTO
				return;
			end
		242:
			begin
				if ((i < NUMBER_OF_VECTOR_THREADS_INT64))
				begin
					state <= 232; // GOTO
					return;
				end
				state <= 233; // GOTO
				return;
			end
		233:
			begin
				toCPU.send_response(pkt);
				state <= 231; // GOTO
				return;
			end
		243:
			begin
				assert((pkt.packet_type == VEC_BUS_PKT_TYPE_write_vec64));
				i <= 0;
				state <= 244; // GOTO
				return;
			end
		244:
			begin
				$display("[VEC-CTRL] store waiting for target bus idle");
				state <= 246; // GOTO
				return;
			end
		246:
			begin
				// CONTEXT_SWITCH();
				state <= 248; // GOTO
				return;
			end
		248:
			begin
				if (toMemory.request_busy)
				begin
					state <= 246; // GOTO
					return;
				end
				state <= 247; // GOTO
				return;
			end
		247:
			begin
				$display("[VEC-CTRL] store vec value ", get(pkt.payload, i), " for address ", pkt.address.data[i]);
				toMemory.send_write_request_data(pkt.address.data[i], pkt.source, get(pkt.payload, i));
				i <= (i + 1);
				// CONTEXT_SWITCH();
				state <= 249; // GOTO
				return;
			end
		249:
			begin
				if ((i < NUMBER_OF_VECTOR_THREADS_INT64))
				begin
					state <= 244; // GOTO
					return;
				end
				state <= 245; // GOTO
				return;
			end
		245:
			begin
				state <= 231; // GOTO
				return;
			end
		231:
			begin
				state <= 229; // GOTO
				return;
			end
		229:
			begin
				// CONTEXT_SWITCH();
				state <= 250; // GOTO
				return;
			end
		250:
			begin
				state <= 227; // GOTO
				return;
			end
		228:
			begin
			end
	endcase
	endtask
endmodule
