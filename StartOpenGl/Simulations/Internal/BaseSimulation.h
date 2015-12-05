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

		virtual void Step(float dT) = 0;

		World::World& World();

	protected:
		std::unique_ptr<World::World> _world;
	};
}

