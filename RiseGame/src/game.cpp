#include "game.h"

#include "utils/file_io.h"


RenderResource render_resources[2] = { {{ 0.2f, 0.2f, 0.8f }, 0}, {{0.2f, 0.8f, 0.4f}, 1} };

// tile map
const uint32_t map_width = 32, map_height = 18;
const uint32_t tile_size = 50;

#define TILE(x) ((x) * tile_size)

uint8_t map[map_height][map_width] = {
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1 },
	{ 1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
};


void game_init()
{
	// TODO : fetch the file with user settings(e.g. screen resolution, display mode (fullscreen, windowed), controls scheme)
	// for now it's hardcoded

	// init the game entities and subsystems

	player.position = {500, 500};

	player.size = {TILE(1), TILE(1)};

	player.render_id = 0;


	test_zombie.position.x = 0;
	test_zombie.position.y = 500;

	test_zombie.size.x = 50;
	test_zombie.size.y = 50;

	test_zombie.velocity = {3, 3};

	test_zombie.render_id = 1;
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

#define FILE_IO_TEST 0
#if FILE_IO_TEST == 1
	char filepath[] = __FILE__;
	void* file_contents = debug_platform_read_file(filepath);
	if (file_contents)
		close_file(file_contents);

	char filepath_2[] = "d:/test_risegame.txt";
	debug_platform_write_file(filepath_2, 21, (void*)"Hello from RiseGame!");
#endif
	
#define AUDIO_TEST 0
#if AUDIO_TEST == 1
	if (game_input->keyboard.keys[RG_UP].is_down && !game_input->keyboard.keys[RG_UP].was_down)
		game_state->sample_volume = 1000;
	else if (game_input->keyboard.keys[RG_DOWN].is_down && !game_input->keyboard.keys[RG_DOWN].was_down)
		game_state->sample_volume = 100;
	
	if (game_input->keyboard.keys[RG_RIGHT].is_down && !game_input->keyboard.keys[RG_RIGHT].was_down)
		game_state->sample_hz = 500;
	else if (game_input->keyboard.keys[RG_LEFT].is_down && !game_input->keyboard.keys[RG_LEFT].was_down)
		game_state->sample_hz = 50;

	outputSound(sound_buffer, game_state->sample_volume, game_state->sample_hz);
#endif

	render_background(graphics_buffer, { 0.0f, 1.0f, 0.5f });
	
	for (int y = 0; y < map_height; y++)
	{
		for (int x = 0; x < map_width; x++)
		{
			uint32_t pixel_x = x * tile_size;
			uint32_t pixel_y = y * tile_size;
			uint32_t pixel_width = x * tile_size + tile_size;
			uint32_t pixel_height = y * tile_size + tile_size;

			if (map[y][x])
				render_rectangle(graphics_buffer, pixel_x, pixel_y, pixel_width, pixel_height, { 1.0f, 0.0f, 0.0f });
			else
				render_rectangle(graphics_buffer, pixel_x, pixel_y, pixel_width, pixel_height, { 0.0f, 0.0f, 1.0f });
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
		{ render_resources[player.render_id].color }
	);

	render_rectangle(
		graphics_buffer,
		test_zombie.position.x,
		test_zombie.position.y,
		test_zombie.position.x + test_zombie.size.x,
		test_zombie.position.y + test_zombie.size.y,
		{ render_resources[test_zombie.render_id].color }
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