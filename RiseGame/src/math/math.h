#pragma once

#include <stdlib.h>


struct vec2
{
	int32_t x, y;
};

static void add(vec2* a, vec2 b)
{
	a->x += b.x;
	a->y += b.y;
}

struct vec3
{
	int32_t x, y, z;
};

struct vec4
{
	int32_t x, y, w, h;
};