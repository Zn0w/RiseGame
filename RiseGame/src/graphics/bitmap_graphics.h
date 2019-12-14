#pragma once

#include <stdint.h>


struct BitmapBuffer
{
	int width, height;
	void* memory;
	int pitch;	// in bytes
};

struct RGBColor
{
	int red;
	int green;
	int blue;
};


static int bytes_per_pixel = 4;


static void render_background(BitmapBuffer* buffer, RGBColor color)
{
	uint8_t* row = (uint8_t*)buffer->memory;
	for (int y = 0; y < buffer->height; y++)
	{
		uint8_t* pixel = (uint8_t*)row;
		for (int x = 0; x < buffer->width; x++)
		{
			// NOTE : little endian architecture
			//		  pixel in memory : BB GG RR xx
			//					  blue  green  red  pad byte
			*pixel = color.blue;
			++pixel;

			*pixel = color.green;
			++pixel;

			*pixel = color.red;
			++pixel;

			*pixel = 0;
			++pixel;
		}
		row += buffer->pitch;
	}
}

static void render_rectangle(BitmapBuffer* buffer, int32_t min_x, int32_t min_y, int32_t max_x, int32_t max_y, RGBColor color)
{
	if (min_x < 0)
		min_x = 0;
	
	if (min_y < 0)
		min_y = 0;

	if (max_x > buffer->width)
		max_x = buffer->width;

	if (max_y > buffer->height)
		max_y = buffer->height;

	uint8_t* row = (uint8_t*)buffer->memory + min_x * bytes_per_pixel + min_y * buffer->pitch;
	for (int y = min_y; y < max_y; y++)
	{
		uint8_t* pixel = (uint8_t*)row;
		for (int x = min_x; x < max_x; x++)
		{
			*pixel = color.blue;
			++pixel;

			*pixel = color.green;
			++pixel;

			*pixel = color.red;
			++pixel;

			*pixel = 0;
			++pixel;
		}
		row += buffer->pitch;
	}
}