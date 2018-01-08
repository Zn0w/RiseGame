#pragma once

enum EntityType
{
	Player,
	Enemy,
	Object
};

struct Entity
{
	int x, y;
	int width, height;
	// Draw/update if true, not if false
	bool active;
	EntityType type;
	const char* id;


	Entity(int xpos, int ypos, int w, int h, bool a, EntityType t)
		: x(xpos), y(ypos), width(w), height(h), active(a), type(t)
	{

	}
};