#pragma once

#include "sound/sound.h"


struct BitmapBuffer
{
	int width, height;
	void* memory;
	int pitch;	// in bytes
};


void game_init();
void game_update_and_render(float time, BitmapBuffer* graphics_buffer, SoundBuffer* sound_buffer);
void game_destroy();

// returns a user-defined resolution
void get_user_resolution(int* screen_width, int* screen_height);