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


static uint32_t roundFloatToUInt32(float value)
{
	return (uint32_t)(value + 0.5f);
}