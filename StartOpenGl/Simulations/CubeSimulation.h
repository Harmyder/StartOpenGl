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

		void Step(float dT) override;

	private:
		World::Box *_box;
	};
}
