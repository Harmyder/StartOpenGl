#include "stdafx.h"

#include "Simulations\CubeSimulation.h"

#include "World\Shapes\Box.h"

using namespace std;

namespace Simulations
{
	CubeSimulation::CubeSimulation() 
	{
	}

	CubeSimulation::~CubeSimulation() 
	{
	}

	void CubeSimulation::Init()
	{
		glm::vec3 halfExtents(1.0, 1.0, 1.0);
		glm::mat4 identity;
		_box = make_unique<World::Box>(halfExtents, identity);
	}

	void CubeSimulation::Step(float dT)
	{
		UNREFERENCED_PARAMETER(dT);
	}
}