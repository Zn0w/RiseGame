#pragma once

#include "../math/math.h"


struct Entity
{
	vec2 position;
	vec2 size;
	vec2 velocity;
	uint32_t render_id;
};


// tl - top left, br - bottom right
static bool collides(vec2 a_tl, vec2 a_br, vec2 b_tl, vec2 b_br)
{
	return	a_tl.x < b_br.x &&
			a_br.x > b_tl.x &&
			a_tl.y < b_br.y &&
			a_br.y > b_tl.y;
}

static bool collides(Entity e1, Entity e2)
{
	// tl - top left
	// br - bottom right
	
	vec2 e1_position_tl = add(e1.position, {-e1.size.x / 2, -e1.size.y / 2});
	vec2 e1_position_br = add(e1.position, { e1.size.x / 2, e1.size.y / 2 });

	vec2 e2_position_tl = add(e2.position, { -e2.size.x / 2, -e2.size.y / 2 });
	vec2 e2_position_br = add(e2.position, { e2.size.x / 2, e2.size.y / 2 });

	return collides(e1_position_tl, e1_position_br, e2_position_tl, e2_position_br);
}

struct TileType
{
	uint32_t id;
	uint32_t render_id;
	void(*update) (Entity*, vec2, vec2) = 0;
};