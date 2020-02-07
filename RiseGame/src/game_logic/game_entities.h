#pragma once

#include <stdint.h>

#include "../math/math.h"
#include "../ecs/entity.h"


// game mechanics characteristics constants
const int32_t PLAYER_SPEED = 1;
const int32_t PLAYER_RELOAD_DURATION = 10;
const int32_t MAX_BULLET_DISTANCE = 50;
const vec2 BULLET_SIZE = { 10, 10 };


struct Player : public Entity
{
	int8_t reload = 0;
	const int8_t speed = PLAYER_SPEED;
};

static void updateZombie(vec2* position, vec2 target, vec2 velocity_scalar)
{
	if (position->x > target.x)
		velocity_scalar.x *= -1;
	if (position->y > target.y)
		velocity_scalar.y *= -1;

	add(position, velocity_scalar);
}


struct Bullet : public Entity
{
	int8_t distance;
	int8_t distance_left;
};

static void update_bullet(Bullet* bullet)
{
	add(&bullet->position, bullet->velocity);
	bullet->distance_left--;
}

static void wall_tile_update(Entity* e, vec2 tile_tl, vec2 tile_br)
{
	//add(&e->position, { -2 * e->velocity.x, -2 * e->velocity.y });
	vec2 e_tl = add(e->position, { -e->size.x / 2, -e->size.y / 2 });
	vec2 e_br = add(e->position, { e->size.x / 2, e->size.y / 2 });
	
	if ((e->velocity.x > 0 && e_br.x < tile_br.x) || (e->velocity.x < 0 && e_tl.x > tile_tl.x))
	{
		e->velocity.x *= -1;
	}

	if ((e->velocity.y > 0 && e_br.y < tile_br.y) || (e->velocity.y < 0 && e_tl.y > tile_tl.y))
	{
		e->velocity.y *= -1;
	}
}