#pragma once

namespace World
{
	class World;
}

namespace Simulations
{
	class BaseSimulation
	{
	public:
		BaseSimulation();
		virtual ~BaseSimulation();

		virtual void Init() = 0;
		virtual void Step(float dT) = 0;

		World::World& World();

	private:
		std::unique_ptr<World::World> _world;
	};
}

