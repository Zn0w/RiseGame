#pragma once

#include "bitmap_graphics.h"


struct Renderable
{
	RGBColor color;	// there will also be texture in the future (and you'd be able to apply both color and texture)
	uint32_t id;	// depending on further implementation (storage of renderables) may not be needed
};