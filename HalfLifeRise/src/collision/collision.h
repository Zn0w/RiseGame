#pragma once

#include "../entities/entity.h"

namespace collision
{
	// Check if two entities collide each other
	bool isColliding(Entity* e1, Entity* e2);
	
	// Check if entity1's side collides entity2
	bool isCollidingRight(Entity* e1, Entity* e2);
	bool isCollidingLeft(Entity* e1, Entity* e2);
	bool isCollidingTop(Entity* e1, Entity* e2);
	bool isCollidingBottom(Entity* e1, Entity* e2);
}