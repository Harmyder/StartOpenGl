#include "stdafx.h"

#include "Simulations\CubeSimulation.h"

#include "World\Shapes\Box.h"
#include "World\World.h"

using namespace std;

namespace Simulations
{
	CubeSimulation::CubeSimulation() 
	{
		glm::vec3 halfExtents(1.0, 1.0, 1.0);
		glm::mat4 identity;

		auto box_up = make_unique<World::Box>(halfExtents, identity);
		_box = box_up.get();
		_world->AddShape(unique_ptr<World::Shape>(box_up.release()));
        _box->SetTransform(glm::translate(_box->GetTransform(), glm::vec3(0, 0, -10)));
    }

	CubeSimulation::~CubeSimulation() 
	{
	}

	void CubeSimulation::Step(float dT)
	{
        _box->SetTransform(glm::rotate(_box->GetTransform(), 0.03f, glm::vec3(0, 1, 0)));
        UNREFERENCED_PARAMETER(dT);
	}
}