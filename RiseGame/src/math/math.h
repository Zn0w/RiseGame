#pragma once

#include <stdint.h>


union vec2
{
	struct {
		int32_t x, y;
	};
	int32_t e[2];
};

union vec3
{
	struct {
		int32_t x, y, z;
	};
	struct {
		int32_t r, g, b;
	};
	int32_t e[3];
};

union vec4
{
	struct {
		int32_t x, y, z, w;
	};
	struct {
		int32_t r, g, b, a;
	};
	int32_t e[4];
};


static void add(vec2* a, vec2 b)
{
	a->x += b.x;
	a->y += b.y;
}

static vec2 add(vec2 a, vec2 b)
{
	return { a.x + b.x, a.y + b.y };
}

static void add(vec3* a, vec3 b)
{
	a->x += b.x;
	a->y += b.y;
	a->z += b.z;
}

static vec3 add(vec3 a, vec3 b)
{
	return { a.x + b.x, a.y + b.y, a.z + b.z };
}

static void add(vec4* a, vec4 b)
{
	a->x += b.x;
	a->y += b.y;
	a->z += b.z;
	a->w += b.w;
}

static vec4 add(vec4 a, vec4 b)
{
	return { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
}

static uint32_t round_float_to_uint32(float value)
{
	return (uint32_t)(value + 0.5f);
}

static int32_t round_float_to_int32(float value)
{
	return (int32_t)(value + 0.5f);
}