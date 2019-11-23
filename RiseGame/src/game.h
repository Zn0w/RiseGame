#pragma once

#include "graphics/bitmap_graphics.h"
#include "sound/sound.h"
#include "input/keyboard.h"


void game_init();
void game_update_and_render(float time, BitmapBuffer* graphics_buffer, SoundBuffer* sound_buffer, KeyboardInputMap* keyboard);
void game_destroy();

// returns a user-defined resolution
void get_user_resolution(int* screen_width, int* screen_height);