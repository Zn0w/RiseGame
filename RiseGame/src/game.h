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


// FOR NOW THE GAME STATE WILL BE STORED IN HERE (STACK) AND ON THE HEAP (WITH new AND delete),
// BUT IT WILL BE STORED USING CUSTOM struct GameMemory
struct GameState
{
	//int16_t sample_volume;
	//int sample_hz;

	Player player;
	
	RenderResource render_resources[6] = {
		{ {},{ 1.0f, 1.0f, 0.0f }, 0 },{ {},{ 1.0f, 0.0f, 1.0f }, 1 },{ {},{ 0.2f, 0.2f, 0.8f }, 2 },{ {},{ 0.2f, 0.8f, 0.4f }, 3 },
		{ {},{ 1.0f, 1.0f, 0.0f }, 4 },{ {} ,{ 0.8f, 0.3f, 0.0f }, 5 }
	};

	TileType tile_types[2] = {
		{0,0,0}, {1,1,0}
	};

	Tilemap tilemap;
	Camera camera;
};
static GameState game_state;


void game_init();
void game_update_and_render(float time, GameMemory* memory, BitmapBuffer* graphics_buffer, SoundBuffer* sound_buffer, GameInput* game_input);
void game_destroy();

// returns a user-defined resolution
void get_user_resolution(int* screen_width, int* screen_height);