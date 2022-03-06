

module CoreInternalMemoryMultiplexer(MemoryBus out, MemoryBus fetch_input, MemoryBus other_input);
	reg[32:0] state = 0;
	BusPacket fetch_pkt;
	BusPacket other_pkt;
	BusPacket out_pkt;



	
	
	task run();
	case (state)
		0:
			begin
				state <= 249; // GOTO
				return;
			end
		249:
			begin
				if (!(fetch_input.request_busy))
				begin
					state <= 251; // GOTO
					return;
				end
				fetch_pkt <= fetch_input.accept_request();
				// CONTEXT_SWITCH();
				state <= 252; // GOTO
				return;
			end
		252:
			begin
				state <= 253; // GOTO
				return;
			end
		253:
			begin
				// CONTEXT_SWITCH();
				state <= 255; // GOTO
				return;
			end
		255:
			begin
				if (out.request_busy)
				begin
					state <= 253; // GOTO
					return;
				end
				state <= 254; // GOTO
				return;
			end
		254:
			begin
				out.send_request(fetch_pkt);
				state <= 251; // GOTO
				return;
			end
		251:
			begin
				if (!(other_input.request_busy))
				begin
					state <= 256; // GOTO
					return;
				end
				other_pkt <= other_input.accept_request();
				// CONTEXT_SWITCH();
				state <= 257; // GOTO
				return;
			end
		257:
			begin
				state <= 258; // GOTO
				return;
			end
		258:
			begin
				// CONTEXT_SWITCH();
				state <= 260; // GOTO
				return;
			end
		260:
			begin
				if (out.request_busy)
				begin
					state <= 258; // GOTO
					return;
				end
				state <= 259; // GOTO
				return;
			end
		259:
			begin
				out.send_request(other_pkt);
				state <= 256; // GOTO
				return;
			end
		256:
			begin
				if (out.response_busy)
				begin
				end
				else
				begin
					state <= 265; // GOTO
					return;
				end
				out_pkt <= out.get_response();
				// CONTEXT_SWITCH();
				state <= 262; // GOTO
				return;
			end
		262:
			begin
				if ((out_pkt.source.within_core_id == COMPONENT_TYPE_FETCH))
				begin
				end
				else
				begin
					state <= 264; // GOTO
					return;
				end
				fetch_input.send_response(out_pkt);
				state <= 263; // GOTO
				return;
			end
		264:
			begin
				other_input.send_response(out_pkt);
				state <= 263; // GOTO
				return;
			end
		263:
			begin
				state <= 261; // GOTO
				return;
			end
		265:
			begin
				// CONTEXT_SWITCH();
				state <= 266; // GOTO
				return;
			end
		266:
			begin
				state <= 261; // GOTO
				return;
			end
		261:
			begin
				state <= 249; // GOTO
				return;
			end
		250:
			begin
			end
	endcase
	endtask
endmodule
