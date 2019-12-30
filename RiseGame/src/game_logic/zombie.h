#pragma once

#include <stdint.h>

#include "../math/math.h"


static void updateZombie(vec2* position, vec2 target, vec2 velocity_scalar)
{
	if (position->x > target.x)
		velocity_scalar.x *= -1;
	if (position->y > target.y)
		velocity_scalar.y *= -1;

	add(position, velocity_scalar);
}