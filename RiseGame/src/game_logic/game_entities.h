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

struct Zombie : public Entity
{
	int8_t hp = 10;
	int8_t speed = 1;
};

static void updateZombie(float time, Zombie* zombie, vec2 target)
{
	if (zombie->position.x > target.x)
		zombie->velocity.x = -zombie->speed;
	else
		zombie->velocity.x = zombie->speed;

	if (zombie->position.y > target.y)
		zombie->velocity.y = -zombie->speed;
	else
		zombie->velocity.y = zombie->speed;

	add(&zombie->position, zombie->velocity);
}


struct Bullet : public Entity
{
	int8_t distance;
	int8_t distance_left;
};

static void update_bullet(float time, Bullet* bullet)
{
	add(&bullet->position, bullet->velocity);
	bullet->distance_left--;
}

static void wall_tile_update(Entity* e, vec2 tile_tl, vec2 tile_br)
{
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