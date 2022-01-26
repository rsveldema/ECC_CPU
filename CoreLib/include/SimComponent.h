#pragma once

#include <string>

#include "Task.h"

#include "SimComponentRegistry.h"

namespace Simulator
{
	class SimComponent : public coro::Task
	{
	private:
		SimComponentRegistry& registry;

	public:
		SimComponent(SimComponentRegistry& registry, const std::string& _name)
			: coro::Task(_name),
			registry(registry)
		{
			registry.registerComponent(this);
		}

		virtual coro::ReturnObject run() = 0;

	};


}