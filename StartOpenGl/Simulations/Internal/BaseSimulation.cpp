#include "stdafx.h"

#include "Simulations\Internal\BaseSimulation.h"

#include "World\World.h"

namespace Simulations
{
	BaseSimulation::BaseSimulation() : _world(new World::World)
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