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
	// tl - top left
	// br - bottom right
	
	vec2 e1_position_tl = add(e1.position, {-e1.size.x / 2, -e1.size.y / 2});
	vec2 e1_position_br = add(e1.position, { e1.size.x / 2, e1.size.y / 2 });

	vec2 e2_position_tl = add(e2.position, { -e2.size.x / 2, -e2.size.y / 2 });
	vec2 e2_position_br = add(e2.position, { e2.size.x / 2, e2.size.y / 2 });

	return	e1_position_tl.x < e2_position_br.x &&
			e1_position_br.x > e2_position_tl.x &&
			e1_position_tl.y < e2_position_br.y &&
			e1_position_br.y > e2_position_tl.y;
}