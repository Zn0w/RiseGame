#include "game.h"


void game_init()
{
	// TODO : fetch the file with user settings(e.g. screen resolution, display mode (fullscreen, windowed), controls scheme)
	// for now it's hardcoded

	// init the game entities and subsystems
}

void game_update_and_render(float time, BitmapBuffer* graphics_buffer, SoundBuffer* sound_buffer, KeyboardInputMap* keyboard)
{
	static int16_t sample_volume = 500;
	static int sample_hz = 256;
	
	if (keyboard->keys[RG_UP].is_down && !keyboard->keys[RG_UP].was_down)
		sample_volume = 1000;
	else if (keyboard->keys[RG_DOWN].is_down && !keyboard->keys[RG_DOWN].was_down)
		sample_volume = 100;
	
	if (keyboard->keys[RG_RIGHT].is_down && !keyboard->keys[RG_RIGHT].was_down)
		sample_hz = 500;
	else if (keyboard->keys[RG_LEFT].is_down && !keyboard->keys[RG_LEFT].was_down)
		sample_hz = 50;

	outputSound(sound_buffer, sample_volume, sample_hz);
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