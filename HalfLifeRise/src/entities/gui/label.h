#pragma once

#include "../entity.h"

struct Label : Entity
{
	Label(int xpos, int ypos, int w, int h, bool a)
		: Entity(xpos, ypos, w, h, a, Gui_label)
	{

	}
};