#pragma once

#include <stdint.h>
#include <vector>

#include "bitmap_graphics.h"
#include "../math/math.h"
#include "../ecs/entity.h"
#include "../utils/file_io.h"


struct RenderResource
{
	Texture texture;
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


static void link_camera(Camera* camera, Entity entity, Tilemap* tilemap)
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
	if (camera->offset_x > tilemap->width * tilemap->tile_size.x - camera->width)
		camera->offset_x = tilemap->width * tilemap->tile_size.x - camera->width;
	if (camera->offset_y > tilemap->height * tilemap->tile_size.y - camera->height)
		camera->offset_y = tilemap->height * tilemap->tile_size.y - camera->height;
}

static void render_tilemap(BitmapBuffer* graphics_buffer, Tilemap* tilemap, Camera camera, RenderResource* render_resources)
{
	// draw tilemap
	for (int y = 0; y <= (camera.height / tilemap->tile_size.y); y++)
	{
		for (int x = 0; x <= (camera.width / tilemap->tile_size.x); x++)
		{	
			// a position of a tile render resource data in the array
			uint32_t mempos = (y + camera.offset_y / tilemap->tile_size.y) * tilemap->width + (x + camera.offset_x / tilemap->tile_size.x);
			
			// an actual coordinates of a tile in the world (i.e. relative to the world)
			uint32_t pixel_x = (mempos % tilemap->width) * tilemap->tile_size.x;
			uint32_t pixel_y = (mempos / tilemap->width) * tilemap->tile_size.y;

			int32_t relative_position_x = pixel_x - camera.offset_x;
			int32_t relative_position_y = pixel_y - camera.offset_y;

			render_rectangle(
				graphics_buffer,
				relative_position_x,
				relative_position_y,
				relative_position_x + tilemap->tile_size.x,
				relative_position_y + tilemap->tile_size.y,
				render_resources[tilemap->tiles[mempos]].color
			);
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
			relative_position_x - entity.size.x / 2,
			relative_position_y - entity.size.y / 2,
			relative_position_x + entity.size.x / 2,
			relative_position_y + entity.size.y / 2,
			resource.color
		);
	}
}

static void render_entity(BitmapBuffer* graphics_buffer, Entity entity, Camera camera, Texture texture)
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

		render_sprite(
			graphics_buffer,
			relative_position_x - entity.size.x / 2,
			relative_position_y - entity.size.y / 2,
			relative_position_x + entity.size.x / 2,
			relative_position_y + entity.size.y / 2,
			texture
		);
	}
}

/*static void render_entities(BitmapBuffer* graphics_buffer, std::vector<Entity>* entities, Camera camera, RenderResource* render_resources)
{
	for (Entity entity : *entities)
	{
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
				relative_position_x - entity.size.x / 2,
				relative_position_y - entity.size.y / 2,
				relative_position_x + entity.size.x / 2,
				relative_position_y + entity.size.y / 2,
				render_resources[entity.render_id].color
			);
		}
	}
}*/