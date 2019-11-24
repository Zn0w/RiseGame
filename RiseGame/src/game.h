#pragma once

#include <assert.h>

#include "memory/memory.h"
#include "graphics/bitmap_graphics.h"
#include "sound/sound.h"
#include "input/keyboard.h"


struct GameState
{
	int16_t sample_volume;
	int sample_hz;
};

void game_init();
void game_update_and_render(float time, GameMemory* memory, BitmapBuffer* graphics_buffer, SoundBuffer* sound_buffer, KeyboardInputMap* keyboard);
void game_destroy();

// returns a user-defined resolution
void get_user_resolution(int* screen_width, int* screen_height);