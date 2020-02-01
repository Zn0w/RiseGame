#pragma once

#include "bitmap_graphics.h"


struct RenderResource
{
	RGBColor color;	// there will also be texture in the future (and you'd be able to apply both color and texture)
	uint32_t id;	// depending on further implementation (storage of renderables), this may not be needed
};