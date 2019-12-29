#pragma once

#include <stdint.h>

#include "../math/math.h"


static void updateZombie(int32_t* x, int32_t* y, int32_t target_x, int32_t target_y, vec2 speed)
{
	if (*x > target_x)
	{
		*x -= speed.x;
	}
	else
		*x += speed.x;

	if (*y > target_y)
	{
		*y -= speed.y;
	}
	else
		*y += speed.y;
}