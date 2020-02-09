#include "game.h"

#include "utils/file_io.h"


// constants for the test gameplay

// tile map constants
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
	1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
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

// camera constants
const uint32_t visible_tiles_x = 16;
const uint32_t visible_tiles_y = 9;


static void create_bullet(vec2 position, vec2 velocity)
{
	game_state.player.reload = PLAYER_RELOAD_DURATION;
	Bullet* bullet = new Bullet;
	bullet->position = position;
	bullet->distance = MAX_BULLET_DISTANCE;
	bullet->distance_left = bullet->distance;
	bullet->render_id = 5;
	bullet->size = BULLET_SIZE;
	bullet->velocity = velocity;
	game_state.bullets.push_back(bullet);
}

static void create_zombie(vec2 position)
{
	Zombie* zombie = new Zombie;
	//zombie->velocity = {zombie->speed, zombie->speed};
	zombie->size = { 50, 50 };
	zombie->render_id = 3;
	zombie->position = position;
	game_state.zombies.push_back(zombie);
}

static vec2 tiles_to_pixels(int32_t x, int32_t y)
{
	return { x * game_state.tilemap.tile_size.x, y * game_state.tilemap.tile_size.y };
}


void game_init()
{
	// TODO : fetch the file with user settings(e.g. screen resolution, display mode (fullscreen, windowed), controls scheme)
	// for now it's hardcoded

	// init the game entities and subsystems

	game_state.tilemap = { map, map_width, map_height,{ screen_width / (int)visible_tiles_x, screen_height / (int)visible_tiles_y } };

	// load assets
	char player_texture_path[] = "test/assets/hero_4.bmp";
	Texture player_texture = load_bmp_texture(player_texture_path);
	game_state.render_resources[4].texture = player_texture;
	

	game_state.player.position = tiles_to_pixels(15, 8);

	game_state.player.size = tiles_to_pixels(1, 1);

	game_state.player.render_id = 2;


	create_zombie({ 1000, 500 });
	create_zombie({ 2000, 800 });
	create_zombie({ 2500, 300 });

	game_state.camera.width = visible_tiles_x * game_state.tilemap.tile_size.x;
	game_state.camera.height = visible_tiles_y * game_state.tilemap.tile_size.y;
	link_camera(&game_state.camera, game_state.player, &game_state.tilemap);

	game_state.tile_types[1].update = &wall_tile_update;
}

static void updatePlayer(float time)
{
	add(&game_state.player.position, game_state.player.velocity);
	
	game_state.player.velocity = {0, 0};

	if (game_state.player.reload)
		game_state.player.reload--;

	// slow collision test for zombies (for the test gameplay)
	for (Zombie* zombie : game_state.zombies)
	{
		if (collides(game_state.player, *zombie))
			game_state.pause = 10;
	}
}

void game_update_and_render(float time, GameMemory* memory, BitmapBuffer* graphics_buffer, SoundBuffer* sound_buffer, GameInput* game_input)
{
#define MEMORY_TEST 0
#if MEMORY_TEST == 1
	assert(sizeof(GameState) <= memory->permanent_storage_size);
	
	GameState* game_state = (GameState*)memory->permanent_storage;
	if (!memory->is_initialized)
	{
		game_state->sample_volume = 500;
		game_state->sample_hz = 256;

		memory->is_initialized = true;
	}
#endif

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
	if (game_input->keyboard.keys[RG_W].is_down && !game_input->keyboard.keys[RG_W].was_down)
		game_state.player.velocity.y += -game_state.player.speed * time;
	else if (game_input->keyboard.keys[RG_S].is_down && !game_input->keyboard.keys[RG_S].was_down)
		game_state.player.velocity.y += game_state.player.speed * time;

	if (game_input->keyboard.keys[RG_A].is_down && !game_input->keyboard.keys[RG_A].was_down)
		game_state.player.velocity.x += -game_state.player.speed * time;
	else if (game_input->keyboard.keys[RG_D].is_down && !game_input->keyboard.keys[RG_D].was_down)
		game_state.player.velocity.x += game_state.player.speed * time;

	if (game_input->keyboard.keys[RG_UP].is_down && !game_input->keyboard.keys[RG_UP].was_down)
	{
		if (game_state.player.reload == 0)
			create_bullet(game_state.player.position, { 0, -15 });
	}
	if (game_input->keyboard.keys[RG_DOWN].is_down && !game_input->keyboard.keys[RG_DOWN].was_down)
		if (game_state.player.reload == 0)
			create_bullet(game_state.player.position, { 0, 15 });
	if (game_input->keyboard.keys[RG_LEFT].is_down && !game_input->keyboard.keys[RG_LEFT].was_down)
		if (game_state.player.reload == 0)
			create_bullet(game_state.player.position, { -15, 0 });
	if (game_input->keyboard.keys[RG_RIGHT].is_down && !game_input->keyboard.keys[RG_RIGHT].was_down)
		if (game_state.player.reload == 0)
			create_bullet(game_state.player.position, { 15, 0 });

	// Game update
	// tiles update (only with player for now)
	for (int y = (game_state.player.position.y - game_state.player.size.y / 2) / game_state.tilemap.tile_size.y; y < map_height && y < (y + 2); y++)
	{
		for (int x = (game_state.player.position.x - game_state.player.size.x / 2) / game_state.tilemap.tile_size.x; x < map_width && x < (x + 2); x++)
		{
			// if collides
			vec2 position_tl = add(game_state.player.position, { -game_state.player.size.x / 2, -game_state.player.size.y / 2 });
			vec2 position_br = add(game_state.player.position, { game_state.player.size.x / 2, game_state.player.size.y / 2 });
			
			vec2 tile_pos_tl = {x*game_state.tilemap.tile_size.x, y*game_state.tilemap.tile_size.y};
			vec2 tile_pos_br = add(tile_pos_tl, { game_state.tilemap.tile_size.x, game_state.tilemap.tile_size.y });

			if (collides(position_tl, position_br, tile_pos_tl, tile_pos_br))
			{
				uint32_t mempos = y * game_state.tilemap.width + x;
				
				if (game_state.tile_types[game_state.tilemap.tiles[mempos]].update)
					game_state.tile_types[game_state.tilemap.tiles[mempos]].update(&game_state.player, tile_pos_tl, tile_pos_br);
				
				//game_state.tilemap.tiles[mempos] = 1;	// leave a purple trail (test)
			}
		}
	}

	updatePlayer(time);
	//updateZombie(time, &game_state.test_zombie.position, game_state.player.position, game_state.test_zombie.velocity);
	for (int i = 0; i < game_state.zombies.size(); i++)
	{
		Zombie* zombie = game_state.zombies.at(i);
		updateZombie(time, zombie, game_state.player.position);
		
		// slow collision test for bullets (for the test gameplay)
		for (Bullet* bullet : game_state.bullets)
		{
			if (collides(*zombie, *bullet))
				zombie->hp--;
		}

		if (zombie->hp <= 0)
		{
			delete zombie;
			game_state.zombies.erase(game_state.zombies.begin() + i);
		}
	}

	for (int i = 0; i < game_state.bullets.size(); i++)
	{
		Bullet* bullet = game_state.bullets.at(i);
		update_bullet(time, bullet);
		if (bullet->distance_left == 0)
		{
			delete bullet;
			game_state.bullets.erase(game_state.bullets.begin() + i);
		}
	}


	render_background(graphics_buffer, { 0.0f, 1.0f, 0.5f });
	link_camera(&game_state.camera, game_state.player, &game_state.tilemap);

	// render game entities

	render_tilemap(graphics_buffer, &game_state.tilemap, game_state.camera, game_state.render_resources);

	render_entity(graphics_buffer, game_state.player, game_state.camera, game_state.render_resources[game_state.player.render_id]);
	//render_entity(graphics_buffer, player, camera, render_resources[4].texture);

	for (Zombie* zombie : game_state.zombies)
	{
		render_entity(graphics_buffer, *zombie, game_state.camera, game_state.render_resources[zombie->render_id]);
	}

	for (Bullet* bullet : game_state.bullets)
	{
		render_entity(graphics_buffer, *bullet, game_state.camera, game_state.render_resources[bullet->render_id]);
	}

	if (game_state.pause)
	{
		render_rectangle(graphics_buffer, 0, 0, screen_width, screen_height, {1.0f, 0.0f, 0.0f});
		game_state.pause--;
	}

	// test texture render
	//char texture_path[] = "test/assets/hero_3.bmp";
	//Texture test_texture = load_bmp_texture(texture_path);
	render_sprite(graphics_buffer, 100, 100, 164, 164, game_state.render_resources[4].texture);
}

void game_destroy()
{
	
}

void get_user_resolution(int* screen_width, int* screen_height)
{
	*screen_width = 1280;
	*screen_height = 720;
}