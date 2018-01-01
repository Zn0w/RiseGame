#pragma once

struct Entity
{
	int x, y, width, height, ground_pos;
	bool active;

	Entity(int xpos, int ypos, int w, int h)
		: x(xpos), y(ypos), width(w), height(h), active(true)
	{

	}
};