#pragma once

#include "../math/math.h"


struct Entity
{
	vec2 position;
	vec2 size;
	vec2 velocity;
	uint8_t color_id;
};