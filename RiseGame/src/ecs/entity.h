#pragma once

#include "../math/math.h"


struct Entity
{
	vec4 dimensions;	// x, y, w, h (NOT x + w, y + h)
	vec3 color;
};