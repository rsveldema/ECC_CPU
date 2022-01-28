#pragma once

#include "Stats.h"

namespace ecc
{
	struct RAM_Config
	{
		unsigned size;
		Cycles read_latency;
		Cycles write_latency;
	};

}