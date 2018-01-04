#pragma once

enum EntityType
{
	Player,
	Zombie,
	Airy,
	Gunner,
	Static_Object,
	Dynamic_Object
};

struct Entity
{
	// Position of entity on 2d canvas
	int x, y;
	// Size of entity
	int width, height;
	// Draw/update if true, not if false
	bool active;
	// Type of entity
	EntityType type;

	Entity(int xpos, int ypos, int w, int h, bool a, EntityType t)
		: x(xpos), y(ypos), width(w), height(h), active(a), type(t)
	{

	}
};