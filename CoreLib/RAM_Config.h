#pragma once

#include "Stats.h"

namespace Simulator
{
	struct RAM_Config
	{
		unsigned size;
		Cycles read_latency;
		Cycles write_latency;
	};

}