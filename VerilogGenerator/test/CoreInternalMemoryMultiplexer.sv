

module CoreInternalMemoryMultiplexer(MemoryBus out, MemoryBus fetch_input, MemoryBus vec_ctrlr_input);
	reg[32:0] state = 0;
	BusPacket fetch_pkt;
	BusPacket other_pkt;
	BusPacket out_pkt;



	
	
	task run();
	case (state)
		0:
			begin
				state <= 251; // GOTO
				return;
			end
		251:
			begin
				if (!(fetch_input.request_busy))
				begin
					state <= 253; // GOTO
					return;
				end
				fetch_pkt <= fetch_input.accept_request();
				// CONTEXT_SWITCH();
				state <= 254; // GOTO
				return;
			end
		254:
			begin
				assert((fetch_pkt.source.within_core_id == COMPONENT_TYPE_FETCH));
				state <= 255; // GOTO
				return;
			end
		255:
			begin
				// CONTEXT_SWITCH();
				state <= 257; // GOTO
				return;
			end
		257:
			begin
				if (out.request_busy)
				begin
					state <= 255; // GOTO
					return;
				end
				state <= 256; // GOTO
				return;
			end
		256:
			begin
				out.send_request(fetch_pkt);
				state <= 253; // GOTO
				return;
			end
		253:
			begin
				if (!(vec_ctrlr_input.request_busy))
				begin
					state <= 258; // GOTO
					return;
				end
				other_pkt <= vec_ctrlr_input.accept_request();
				// CONTEXT_SWITCH();
				state <= 259; // GOTO
				return;
			end
		259:
			begin
				assert((other_pkt.source.within_core_id != COMPONENT_TYPE_FETCH));
				state <= 260; // GOTO
				return;
			end
		260:
			begin
				// CONTEXT_SWITCH();
				state <= 262; // GOTO
				return;
			end
		262:
			begin
				if (out.request_busy)
				begin
					state <= 260; // GOTO
					return;
				end
				state <= 261; // GOTO
				return;
			end
		261:
			begin
				$display("[CORE_INT_MULTIPLX] sending request from vec-ctrl");
				out.send_request(other_pkt);
				state <= 258; // GOTO
				return;
			end
		258:
			begin
				if (out.response_busy)
				begin
				end
				else
				begin
					state <= 267; // GOTO
					return;
				end
				out_pkt <= out.get_response();
				// CONTEXT_SWITCH();
				state <= 264; // GOTO
				return;
			end
		264:
			begin
				$display("got pkt for component ", out_pkt.source.within_core_id);
				if ((out_pkt.source.within_core_id == COMPONENT_TYPE_FETCH))
				begin
				end
				else
				begin
					state <= 266; // GOTO
					return;
				end
				fetch_input.send_response(out_pkt);
				state <= 265; // GOTO
				return;
			end
		266:
			begin
				vec_ctrlr_input.send_response(out_pkt);
				state <= 265; // GOTO
				return;
			end
		265:
			begin
				state <= 263; // GOTO
				return;
			end
		267:
			begin
				// CONTEXT_SWITCH();
				state <= 268; // GOTO
				return;
			end
		268:
			begin
				state <= 263; // GOTO
				return;
			end
		263:
			begin
				state <= 251; // GOTO
				return;
			end
		252:
			begin
			end
	endcase
	endtask
endmodule
