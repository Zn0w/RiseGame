#pragma once

#include <assert.h>

#include "memory/memory.h"
#include "graphics/bitmap_graphics.h"
#include "sound/sound.h"
#include "input/input.h"

#include "graphics/renderable.h"

#include "ecs/entity.h"

#include "game_logic/zombie.h"


struct GameState
{
	int16_t sample_volume;
	int sample_hz;
};

static Entity player;
static Entity test_zombie;
void updatePlayer(vec2*);

void game_init();
void game_update_and_render(float time, GameMemory* memory, BitmapBuffer* graphics_buffer, SoundBuffer* sound_buffer, GameInput* game_input);
void game_destroy();

// returns a user-defined resolution
void get_user_resolution(int* screen_width, int* screen_height);