#pragma once

#include <stdint.h>

#include "../math/math.h"
#include "../ecs/entity.h"


// game mechanics characteristics constants
const float PLAYER_SPEED = 1.0f;
const int32_t PLAYER_RELOAD_DURATION = 10;
const int32_t MAX_BULLET_DISTANCE = 50;
const vec2 BULLET_SIZE = { 10, 10 };


struct Player : public Entity
{
	int8_t reload = 0;
	const float speed = PLAYER_SPEED;
};

struct Zombie : public Entity
{
	int8_t hp = 10;
	float speed = 0.5f;
};

struct Bullet : public Entity
{
	int8_t distance;
	int8_t distance_left;
	float speed = 2.0f;
};


static void updateZombie(float time, Zombie* zombie, vec2 target)
{
	if (zombie->position.x > target.x)
		zombie->velocity.x = -zombie->speed * time;
	else
		zombie->velocity.x = zombie->speed * time;

	if (zombie->position.y > target.y)
		zombie->velocity.y = -zombie->speed * time;
	else
		zombie->velocity.y = zombie->speed * time;

	add(&zombie->position, zombie->velocity);
}

static void update_bullet(float time, Bullet* bullet)
{
	vec2 vel_with_dt = { bullet->velocity.x * time, bullet->velocity.y * time };
	add(&bullet->position, vel_with_dt);
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