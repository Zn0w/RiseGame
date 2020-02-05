#pragma once

#include "../math/math.h"


struct Entity
{
	vec2 position;
	vec2 size;
	vec2 velocity;
	uint32_t render_id;
};


static bool collides(Entity e1, Entity e2)
{
	return	e1.position.x < e2.position.x + e2.size.x &&
			e1.position.x + e1.size.x > e2.position.x &&
			e1.position.y < e2.position.y + e2.size.y &&
			e1.position.y + e1.size.y > e2.position.y;
}