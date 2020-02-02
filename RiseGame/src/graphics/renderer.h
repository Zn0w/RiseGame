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
	camera->origin_x = entity.position.x;
	camera->origin_y = entity.position.y;

	// top left corner position
	camera->offset_x = camera->origin_x - (camera->width / 2);
	camera->offset_y = camera->origin_y - (camera->height / 2);

	if (camera->offset_x < 0)
		camera->offset_x = 0;
	if (camera->offset_y < 0)
		camera->offset_y = 0;
	if (camera->offset_x > tilemap.width * tilemap.tile_size.x - camera->width)
		camera->offset_x = tilemap.width * tilemap.tile_size.x - camera->width;
	if (camera->offset_y > tilemap.height * tilemap.tile_size.y - camera->height)
		camera->offset_y = tilemap.height * tilemap.tile_size.y - camera->height;
}

static void render_tilemap(Tilemap* tilemap, BitmapBuffer* graphics_buffer, Camera camera)
{
	// draw tilemap
	for (int y = 0; y < (camera.height / tilemap->tile_size.y); y++)
	{
		for (int x = 0; x < (camera.width / tilemap->tile_size.x); x++)
		{
			uint32_t pixel_x = x * tilemap->tile_size.x;
			uint32_t pixel_y = y * tilemap->tile_size.y;
			uint32_t pixel_width = x * tilemap->tile_size.x + tilemap->tile_size.x;
			uint32_t pixel_height = y * tilemap->tile_size.y + tilemap->tile_size.y;

			if (tilemap->tiles[(y + camera.offset_y / tilemap->tile_size.y) * tilemap->width + (x + camera.offset_x / tilemap->tile_size.y)])
				render_rectangle(graphics_buffer, pixel_x, pixel_y, pixel_width, pixel_height, { 1.0f, 0.0f, 0.0f });
			else
				render_rectangle(graphics_buffer, pixel_x, pixel_y, pixel_width, pixel_height, { 0.0f, 0.0f, 1.0f });
		}
	}
}

static void render_entity(BitmapBuffer* graphics_buffer, Entity entity, Camera camera, RenderResource resource)
{
	// render entity relative to camera, not the world
	if (
		entity.position.x < camera.offset_x + camera.width &&
		entity.position.x + entity.size.x > camera.offset_x &&
		entity.position.y < camera.offset_y + camera.height &&
		entity.position.y + entity.size.y > camera.offset_y
		)
	{
		int32_t relative_position_x = entity.position.x - camera.offset_x;
		int32_t relative_position_y = entity.position.y - camera.offset_y;

		render_rectangle(
			graphics_buffer,
			relative_position_x,
			relative_position_y,
			relative_position_x + entity.size.x,
			relative_position_y + entity.size.y,
			{ resource.color }
		);
	}
}

static void render_entities(std::vector<Entity>* entities, Camera camera)
{
	
}