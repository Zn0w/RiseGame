#include "game.h"

#include "utils/file_io.h"


void game_init()
{
	// TODO : fetch the file with user settings(e.g. screen resolution, display mode (fullscreen, windowed), controls scheme)
	// for now it's hardcoded

	// init the game entities and subsystems

	player.dimensions.x = 500;
	player.dimensions.y = 500;

	player.dimensions.w = 50;
	player.dimensions.h = 50;

	player.color.x = 100;
	player.color.y = 100;
	player.color.z = 230;

	test_zombie.dimensions.x = 0;
	test_zombie.dimensions.y = 500;

	test_zombie.dimensions.w = 50;
	test_zombie.dimensions.h = 50;

	test_zombie.color.x = 30;
	test_zombie.color.y = 200;
	test_zombie.color.z = 100;
}

void updatePlayer(vec2* speed)
{
	player.dimensions.x += speed->x;
	player.dimensions.y += speed->y;
}

void game_update_and_render(float time, GameMemory* memory, BitmapBuffer* graphics_buffer, SoundBuffer* sound_buffer, GameInput* game_input)
{
	assert(sizeof(GameState) <= memory->permanent_storage_size);
	
	GameState* game_state = (GameState*)memory->permanent_storage;
	if (!memory->is_initialized)
	{
		game_state->sample_volume = 500;
		game_state->sample_hz = 256;

		memory->is_initialized = true;
	}

	char filepath[] = __FILE__;
	void* file_contents = debug_platform_read_file(filepath);
	if (file_contents)
		close_file(file_contents);

	char filepath_2[] = "d:/test_risegame.txt";
	debug_platform_write_file(filepath_2, 21, (void*)"Hello from RiseGame!");

	
	if (game_input->keyboard.keys[RG_UP].is_down && !game_input->keyboard.keys[RG_UP].was_down)
		game_state->sample_volume = 1000;
	else if (game_input->keyboard.keys[RG_DOWN].is_down && !game_input->keyboard.keys[RG_DOWN].was_down)
		game_state->sample_volume = 100;
	
	if (game_input->keyboard.keys[RG_RIGHT].is_down && !game_input->keyboard.keys[RG_RIGHT].was_down)
		game_state->sample_hz = 500;
	else if (game_input->keyboard.keys[RG_LEFT].is_down && !game_input->keyboard.keys[RG_LEFT].was_down)
		game_state->sample_hz = 50;

	outputSound(sound_buffer, game_state->sample_volume, game_state->sample_hz);
	render_background(graphics_buffer, { 100, 150, 200 });

	// draw a tile map
	uint8_t map[9][16] = {
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};

	uint8_t size = 80;
	for (int y = 0; y < 9; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			if (map[y][x])
				render_rectangle(graphics_buffer, x * size, y * size, x * size + size, y * size + size, { 100, 250, 100 });
			else
				render_rectangle(graphics_buffer, x * size, y * size, x * size + size, y * size + size, { 250, 100, 100 });
		}
	}

	vec2 speed = { 0, 1 }; // 1 on y is gravity
	if (game_input->keyboard.keys[RG_UP].is_down && !game_input->keyboard.keys[RG_UP].was_down)
		speed.y += -5;
	else if (game_input->keyboard.keys[RG_DOWN].is_down && !game_input->keyboard.keys[RG_DOWN].was_down)
		speed.y += 5;

	if (game_input->keyboard.keys[RG_LEFT].is_down && !game_input->keyboard.keys[RG_LEFT].was_down)
		speed.x += -5;
	else if (game_input->keyboard.keys[RG_RIGHT].is_down && !game_input->keyboard.keys[RG_RIGHT].was_down)
		speed.x += 5;

	vec2 zombie_speed = {3, 3};

	updatePlayer(&speed);
	updateZombie(&test_zombie.dimensions.x, &test_zombie.dimensions.y, player.dimensions.x, player.dimensions.y, zombie_speed);

	render_rectangle(
		graphics_buffer,
		player.dimensions.x,
		player.dimensions.y,
		player.dimensions.x + player.dimensions.w,
		player.dimensions.y + player.dimensions.h,
		{ player.color.x, player.color.y, player.color.z }
	);

	render_rectangle(
		graphics_buffer,
		test_zombie.dimensions.x,
		test_zombie.dimensions.y,
		test_zombie.dimensions.x + test_zombie.dimensions.w,
		test_zombie.dimensions.y + test_zombie.dimensions.h,
		{ test_zombie.color.x, test_zombie.color.y, test_zombie.color.z }
	);
}

void game_destroy()
{
	
}

void get_user_resolution(int* screen_width, int* screen_height)
{
	*screen_width = 1280;
	*screen_height = 720;
}