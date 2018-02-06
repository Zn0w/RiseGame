#pragma once

#include <string>

#include "../entity.h"

// Will probably be moved to a common for all of the gui components file
enum AlignType
{
	Right,
	Left,
	Center
};

struct Label : Entity
{
	AlignType align;
	std::string text;
	
	Label(int xpos, int ypos, int w, int h, bool a, AlignType al)
		: Entity(xpos, ypos, w, h, a, Gui_label), align(al)
	{

	}
};