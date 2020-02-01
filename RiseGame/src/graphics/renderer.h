#pragma once

#include <stdint.h>
#include <vector>

#include "bitmap_graphics.h"
#include "../math/math.h"
#include "../ecs/entity.h"


struct RenderResource
{
	RGBColor color;	// there will also be texture in the future (and you'd be able to apply both color and texture)
	uint32_t id;	// depending on further implementation (storage of renderables), this may not be needed
};

// Tilemap camera, 1 unit = 1 tile
struct Camera
{
	int32_t origin_x, origin_y;
	int32_t width, height;
	int32_t offset_x, offset_y;
};

struct Tilemap
{
	uint32_t* tiles;
	uint32_t width;
	uint32_t height;
	vec2 tile_size;	// in pixels
};


static void link_camera(Camera* camera, Entity entity, Tilemap tilemap)
{
	camera->origin_x = entity.position.x / tilemap.tile_size.x;
	camera->origin_y = entity.position.y / tilemap.tile_size.y;

	// top left corner position
	camera->offset_x = camera->origin_x - (camera->width / 2);
	camera->offset_y = camera->origin_y - (camera->height / 2);

	if (camera->offset_x < 0)
		camera->offset_x = 0;
	if (camera->offset_y < 0)
		camera->offset_y = 0;
	if (camera->offset_x > tilemap.width - camera->width)
		camera->offset_x = tilemap.width - camera->width;
	if (camera->offset_y > tilemap.height - camera->height)
		camera->offset_y = tilemap.height - camera->height;
}

static void render_tilemap(Tilemap* tilemap, BitmapBuffer* graphics_buffer, Camera camera)
{
	// draw tilemap
	for (int y = 0; y < camera.height; y++)
	{
		for (int x = 0; x < camera.width; x++)
		{
			uint32_t pixel_x = x * tilemap->tile_size.x;
			uint32_t pixel_y = y * tilemap->tile_size.y;
			uint32_t pixel_width = x * tilemap->tile_size.x + tilemap->tile_size.x;
			uint32_t pixel_height = y * tilemap->tile_size.y + tilemap->tile_size.y;

			if (tilemap->tiles[(y + camera.offset_y) * tilemap->width + (x + camera.offset_x)])
				render_rectangle(graphics_buffer, pixel_x, pixel_y, pixel_width, pixel_height, { 1.0f, 0.0f, 0.0f });
			else
				render_rectangle(graphics_buffer, pixel_x, pixel_y, pixel_width, pixel_height, { 0.0f, 0.0f, 1.0f });
		}
	}
}

static void render_entities(std::vector<Entity>* entities, Camera camera)
{
	
}