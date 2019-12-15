#pragma once


struct vec2
{
	int x, y;
};

static void add(vec2* a, vec2 b)
{
	a->x += b.x;
	a->y += b.y;
}

struct vec3
{
	int x, y, z;
};