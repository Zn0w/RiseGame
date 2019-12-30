#include "game.h"

#include "utils/file_io.h"


RGBColor colors[2] = { { 50, 100, 230 }, {30, 200, 100} };

void game_init()
{
	// TODO : fetch the file with user settings(e.g. screen resolution, display mode (fullscreen, windowed), controls scheme)
	// for now it's hardcoded

	// init the game entities and subsystems

	player.position.x = 500;
	player.position.y = 500;

	player.size.x = 50;
	player.size.y = 50;

	player.color_id = 0;


	test_zombie.position.x = 0;
	test_zombie.position.y = 500;

	test_zombie.size.x = 50;
	test_zombie.size.y = 50;

	test_zombie.velocity = { 3, 3 };

	test_zombie.color_id = 1;
}

void updatePlayer()
{
	add(&player.position, player.velocity);
	
	// gravity
	player.velocity = {0, 1};
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

	if (game_input->keyboard.keys[RG_UP].is_down && !game_input->keyboard.keys[RG_UP].was_down)
		player.velocity.y += -5;
	else if (game_input->keyboard.keys[RG_DOWN].is_down && !game_input->keyboard.keys[RG_DOWN].was_down)
		player.velocity.y += 5;

	if (game_input->keyboard.keys[RG_LEFT].is_down && !game_input->keyboard.keys[RG_LEFT].was_down)
		player.velocity.x += -5;
	else if (game_input->keyboard.keys[RG_RIGHT].is_down && !game_input->keyboard.keys[RG_RIGHT].was_down)
		player.velocity.x += 5;

	updatePlayer();
	updateZombie(&test_zombie.position, player.position, test_zombie.velocity);

	render_rectangle(
		graphics_buffer,
		player.position.x,
		player.position.y,
		player.position.x + player.size.x,
		player.position.y + player.size.y,
		{ colors[player.color_id] }
	);

	render_rectangle(
		graphics_buffer,
		test_zombie.position.x,
		test_zombie.position.y,
		test_zombie.position.x + test_zombie.size.x,
		test_zombie.position.y + test_zombie.size.y,
		{ colors[test_zombie.color_id] }
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