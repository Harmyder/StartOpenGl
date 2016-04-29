#include "stdafx.h"

#include "Simulations\CubeSimulation.h"

#include "World\Shapes\Box.h"
#include "World\World.h"

using namespace std;

namespace Simulations
{
	CubeSimulation::CubeSimulation() 
	{
		glm::vec3 halfExtents(1.f, 1.f, 1.f);
		glm::mat4 identity;

		auto box = make_unique<World::Box>(halfExtents, identity);
		_box_top = box.get();
		_world->AddShape(unique_ptr<World::Shape>(box.release()));

        box = make_unique<World::Box>(halfExtents * 2.f, identity);
        _box_bottom = box.get();
        _world->AddShape(unique_ptr<World::Shape>(box.release()));

        _box_top->SetTransform(glm::translate(_box_top->GetTransform(), glm::vec3(0, 3, -10)));
        _box_bottom->SetTransform(glm::translate(_box_bottom->GetTransform(), glm::vec3(0, -3, -10)));
    }

	CubeSimulation::~CubeSimulation() 
	{
	}

	void CubeSimulation::Step(float dT)
	{
        _box_top->SetTransform(glm::rotate(_box_top->GetTransform(), 0.03f, glm::vec3(0, 1, 0)));
        _box_bottom->SetTransform(glm::rotate(_box_bottom->GetTransform(), -0.03f, glm::vec3(0, 1, 0)));
        UNREFERENCED_PARAMETER(dT);
	}
}