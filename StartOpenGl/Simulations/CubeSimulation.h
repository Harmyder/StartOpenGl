#pragma once

#include "Simulations\Internal\BaseSimulation.h"

namespace World
{
	class Box;
}

namespace Simulations
{
	class CubeSimulation : public BaseSimulation
	{
	public:
		CubeSimulation();
		~CubeSimulation() override;

		void Init() override;
		void Step(float dT) override;

	private:
		std::unique_ptr<World::Box> _box;
	};
}
