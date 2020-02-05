#include "game.h"

#include "utils/file_io.h"


RenderResource render_resources[6] = {
	{ {}, { 1.0f, 1.0f, 0.0f }, 0 }, { {}, { 1.0f, 0.0f, 1.0f }, 1 }, { {}, { 0.2f, 0.2f, 0.8f }, 2}, { {}, {0.2f, 0.8f, 0.4f}, 3},
	{ {}, { 1.0f, 1.0f, 0.0f }, 4 }, { {} ,{ 0.8f, 0.3f, 0.0f }, 5 }
};

// tile map
const uint32_t map_width = 64, map_height = 32;

uint32_t map[map_height * map_width] = {
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,
	1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};

const uint32_t visible_tiles_x = 16;
const uint32_t visible_tiles_y = 9;

Tilemap tilemap = { map, map_width, map_height, {screen_width / visible_tiles_x, screen_height / visible_tiles_y } };


Camera camera;


// Game entities
static Player player;
static Entity test_zombie;
static std::vector<Bullet*> bullets;
int8_t pause = 0;


static void create_bullet(vec2 position, vec2 velocity)
{
	player.reload = 10;
	Bullet* bullet = new Bullet;
	bullet->position = position;
	bullet->distance = 50;
	bullet->distance_left = bullet->distance;
	bullet->render_id = 5;
	bullet->size = { 10, 10 };
	bullet->velocity = velocity;
	bullets.push_back(bullet);
}

static vec2 tiles_to_pixels(int32_t x, int32_t y)
{
	return { x * tilemap.tile_size.x, y * tilemap.tile_size.y };
}


void game_init()
{
	// TODO : fetch the file with user settings(e.g. screen resolution, display mode (fullscreen, windowed), controls scheme)
	// for now it's hardcoded

	// init the game entities and subsystems

	// load assets
	char player_texture_path[] = "test/assets/hero_4.bmp";
	Texture player_texture = load_bmp_texture(player_texture_path);
	render_resources[4].texture = player_texture;
	

	player.position = tiles_to_pixels(15, 8);

	player.size = tiles_to_pixels(1, 1);
	//player.size = { 64, 64 };

	player.render_id = 2;


	test_zombie.position.x = 0;
	test_zombie.position.y = 500;

	test_zombie.size.x = 50;
	test_zombie.size.y = 50;

	test_zombie.velocity = {3, 3};

	test_zombie.render_id = 3;

	camera.width = visible_tiles_x * tilemap.tile_size.x;
	camera.height = visible_tiles_y * tilemap.tile_size.y;
	link_camera(&camera, player, &tilemap);
}

static void updatePlayer()
{
	add(&player.position, player.velocity);
	
	player.velocity = {0, 0};

	if (player.reload)
		player.reload--;

	if (collides(player, test_zombie))
		pause = 10;
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


	// Get player input
	const int32_t player_speed = 10;
	if (game_input->keyboard.keys[RG_W].is_down && !game_input->keyboard.keys[RG_W].was_down)
		player.velocity.y += -player_speed;
	else if (game_input->keyboard.keys[RG_S].is_down && !game_input->keyboard.keys[RG_S].was_down)
		player.velocity.y += player_speed;

	if (game_input->keyboard.keys[RG_A].is_down && !game_input->keyboard.keys[RG_A].was_down)
		player.velocity.x += -player_speed;
	else if (game_input->keyboard.keys[RG_D].is_down && !game_input->keyboard.keys[RG_D].was_down)
		player.velocity.x += player_speed;

	if (game_input->keyboard.keys[RG_UP].is_down && !game_input->keyboard.keys[RG_UP].was_down)
	{
		if (player.reload == 0)
			create_bullet(player.position, { 0, -15 });
	}
	if (game_input->keyboard.keys[RG_DOWN].is_down && !game_input->keyboard.keys[RG_DOWN].was_down)
		if (player.reload == 0)
			create_bullet(player.position, { 0, 15 });
	if (game_input->keyboard.keys[RG_LEFT].is_down && !game_input->keyboard.keys[RG_LEFT].was_down)
		if (player.reload == 0)
			create_bullet(player.position, { -15, 0 });
	if (game_input->keyboard.keys[RG_RIGHT].is_down && !game_input->keyboard.keys[RG_RIGHT].was_down)
		if (player.reload == 0)
			create_bullet(player.position, { 15, 0 });

	// Game update
	updatePlayer();
	updateZombie(&test_zombie.position, player.position, test_zombie.velocity);

	for (int i = 0; i < bullets.size(); i++)
	{
		Bullet* bullet = bullets.at(i);
		update_bullet(bullet);
		if (bullet->distance_left == 0)
		{
			delete bullet;
			bullets.erase(bullets.begin() + i);
		}
	}


	render_background(graphics_buffer, { 0.0f, 1.0f, 0.5f });
	link_camera(&camera, player, &tilemap);

	// render game entities

	render_tilemap(graphics_buffer, &tilemap, camera, render_resources);

	render_entity(graphics_buffer, player, camera, render_resources[player.render_id]);
	//render_entity(graphics_buffer, player, camera, render_resources[4].texture);

	render_entity(graphics_buffer, test_zombie, camera, render_resources[test_zombie.render_id]);

	for (Bullet* bullet : bullets)
	{
		render_entity(graphics_buffer, *bullet, camera, render_resources[bullet->render_id]);
	}

	if (pause)
	{
		render_rectangle(graphics_buffer, 0, 0, screen_width, screen_height, {1.0f, 0.0f, 0.0f});
		pause--;
	}

	// test texture render
	//char texture_path[] = "test/assets/hero_3.bmp";
	//Texture test_texture = load_bmp_texture(texture_path);
	render_sprite(graphics_buffer, 100, 100, 164, 164, render_resources[4].texture);
}

void game_destroy()
{
	
}

void get_user_resolution(int* screen_width, int* screen_height)
{
	*screen_width = 1280;
	*screen_height = 720;
}