#include "game.h"

#include <stdint.h>


void game_init()
{
	// TODO : fetch the file with user settings(e.g. screen resolution, display mode (fullscreen, windowed), controls scheme)
	// for now it's hardcoded

	// init the game entities and subsystems
}

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

void game_update_and_render(float time, BitmapBuffer* graphics_buffer, SoundBuffer* sound_buffer)
{
	outputSound(sound_buffer);
	render_background(graphics_buffer, 100, 150, 200);
}

void game_destroy()
{
	
}

void get_user_resolution(int* screen_width, int* screen_height)
{
	*screen_width = 1280;
	*screen_height = 720;
}