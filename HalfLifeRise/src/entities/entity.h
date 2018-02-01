#pragma once

enum EntityType
{
	Player,
	Enemy,
	Object,
	Ground,
	Gui_label,
	Gui_button
};

struct Entity
{
	double x, y;
	int width, height;
	// Draw/update if true, not if false
	bool active;
	EntityType type;


	Entity(int xpos, int ypos, int w, int h, bool a, EntityType t)
		: x(xpos), y(ypos), width(w), height(h), active(a), type(t)
	{

	}

	~Entity()
	{
		
	}
};