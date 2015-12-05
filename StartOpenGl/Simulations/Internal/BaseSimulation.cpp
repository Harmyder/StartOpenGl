#include "stdafx.h"

#include "Simulations\Internal\BaseSimulation.h"

#include "World\World.h"

using namespace std;

namespace Simulations
{
	BaseSimulation::BaseSimulation() : _world(make_unique<World::World>())
	{
	}

	BaseSimulation::~BaseSimulation()
	{
	}

	World::World& BaseSimulation::World()
	{ 
		return *_world; 
	}

}