#include "stdafx.h"

#include "World\World.h"

#include "World\Shapes\Shape.h"

using namespace std;

namespace World
{
	World::World() 
	{
	}

	World::~World() 
	{
	}

	void World::AddShape(unique_ptr<Shape> shape)
	{
		_shapes.push_back(move(shape));
	}
}
