#pragma once

struct Entity
{
	int x, y, width, height;
	int width, height;
	bool active;

	Entity(int xpos, int ypos, int w, int h, bool a)
		: x(xpos), y(ypos), width(w), height(h), active(a)
	{

	}
};