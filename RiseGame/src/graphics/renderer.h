#pragma once

#include <stdint.h>
#include <vector>

#include "bitmap_graphics.h"
#include "../ecs/entity.h"


struct RenderResource
{
	RGBColor color;	// there will also be texture in the future (and you'd be able to apply both color and texture)
	uint32_t id;	// depending on further implementation (storage of renderables), this may not be needed
};

// Tilemap camera, 1 unit = 1 tile
struct Camera
{
	uint32_t origin_x, origin_y;
	uint32_t width, height;
	uint32_t offset_x, offset_y;
};


static void render_entities(std::vector<Entity>* entities, Camera camera)
{
	
}