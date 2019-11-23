#pragma once

#include <stdint.h>


struct BitmapBuffer
{
	int width, height;
	void* memory;
	int pitch;	// in bytes
};


static void render_background(BitmapBuffer* buffer, int red, int green, int blue)
{
	int bytes_per_pixel = 4;
	int pitch = buffer->width * bytes_per_pixel;
	uint8_t* row = (uint8_t*)buffer->memory;
	for (int y = 0; y < buffer->height; y++)
	{
		uint8_t* pixel = (uint8_t*)row;
		for (int x = 0; x < buffer->width; x++)
		{
			// NOTE : little endian architecture
			//		  pixel in memory : BB GG RR xx
			//					  blue  green  red  pad byte
			*pixel = blue;
			++pixel;

			*pixel = green;
			++pixel;

			*pixel = red;
			++pixel;

			*pixel = 0;
			++pixel;
		}
		row += pitch;
	}
}