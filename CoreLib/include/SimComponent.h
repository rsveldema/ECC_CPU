#pragma once

#include "task.h"

#include "SimComponentRegistry.h"

namespace Simulator
{
	using byte = unsigned char;


	class SimComponent : public coro::Task
	{
	private:
		SimComponentRegistry& registry;

	public:
		const std::string name;

	public:
		SimComponent(SimComponentRegistry& registry, const std::string& name)
			: registry(registry),
			name(name)
		{
			registry.registerComponent(this);
		}

		virtual coro::ReturnObject run() = 0;

	};


}