#pragma once

namespace Simulator
{
	using byte = unsigned char;

	class SimComponent;

	class SimComponentRegistry
	{
	private:
		std::vector<SimComponent*> components;

	public:
		void registerComponent(SimComponent* component)
		{
			components.push_back(component);
		}

		void run()
		{

		}
	};

	class SimComponent
	{
	public:
		SimComponent(SimComponentRegistry& registry)
		{
			registry.registerComponent(this);
		}

		virtual void run() = 0;

		void waitForTick()
		{
		}

		void start()
		{

		}
	};


}