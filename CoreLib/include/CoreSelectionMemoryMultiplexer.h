#pragma once

#include <functional>
#include "MemoryBus.h"
#include "SimComponent.h"


namespace ecc
{
	/** @brief N-inputs and 1-output.
	* When some data comes back up (a reponse), the multiplexer also de-multiplexes to send the reply to
	* the correct up-stream input.
	*/
	class CoreSelectionMemoryMultiplexer : public SimComponent
	{
	public:
		using PacketType = BusPacket;

		static constexpr uint32_t NUM_CORES = 1;

	private:
		MemoryBus& _out;

		/** @brief N inputs
		*/
		std::array<MemoryBus*, NUM_CORES> _inputs;


	public:		
	
		void addInput(unsigned ix, MemoryBus* input)
		{
			_inputs[ix] = input;
		}

		CoreSelectionMemoryMultiplexer(SimComponentRegistry& registry, MemoryBus& _out)
			: SimComponent(registry, "multiplexer"),
			_out(_out)
		{
		}

		ReturnObject run() override {
			return run(_out, _inputs);
		}

		ReturnObject run(MemoryBus& out, std::array<MemoryBus*, NUM_CORES>& inputs);
	};

}