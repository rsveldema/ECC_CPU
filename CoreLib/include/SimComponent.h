#pragma once

#include <string>

#include "Task.h"

#include "SimComponentRegistry.h"

namespace ecc
{
	class SimComponent : public ecc::Task
	{
	private:
		SimComponentRegistry& registry;

	public:
		SimComponent(SimComponentRegistry& registry, const std::string& _name)
			: ecc::Task(_name),
			registry(registry)
		{
			registry.registerComponent(this);
		}

		virtual ecc::ReturnObject run() = 0;

	};


}