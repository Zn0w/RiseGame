#pragma once

#include <assert.h>

#include "memory/memory.h"
#include "graphics/bitmap_graphics.h"
#include "sound/sound.h"
#include "input/input.h"

#include "graphics/renderer.h"

#include "ecs/entity.h"

#include "game_logic/game_entities.h"


static int screen_width = 1600;
static int screen_height = 900;


struct GameState
{
	int16_t sample_volume;
	int sample_hz;
};


void game_init();
void game_update_and_render(float time, GameMemory* memory, BitmapBuffer* graphics_buffer, SoundBuffer* sound_buffer, GameInput* game_input);
void game_destroy();

// returns a user-defined resolution
void get_user_resolution(int* screen_width, int* screen_height);