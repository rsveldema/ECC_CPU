#pragma once

#include "Stats.h"
#include "RAM_Config.h"


namespace Simulator
{
	struct MachineConfig
	{
		RAM_Config grid_mem_config;
		RAM_Config sram_config;
		unsigned num_cores;
	};

}