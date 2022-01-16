#pragma once

#include "task.h"
#include <vector>

namespace Simulator
{
	using byte = unsigned char;
	class Machine;

	class SimComponent;

	class SimComponentRegistry
	{
	public:
		std::vector<SimComponent*> components;

	public:
		void registerComponent(SimComponent* component)
		{
			components.push_back(component);
		}

		void run(Machine& machine);
	};

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