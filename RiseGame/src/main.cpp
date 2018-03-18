#include <vector>
#include <iostream>

#include "graphics/render.h"

#include "entity/entity.h"
#include "entity/update.h"

#include "button/button.h"
#include "button/events.h"

enum GameState
{
	Menu,
	Pause,
	Running
};

bool game_running = false;
GameState current_state;

void update(std::vector<Entity*>*, float);
void update(std::vector<Button*>*);
void destroy(sf::RenderWindow*);

int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!");
	window.setFramerateLimit(60);
	sf::Clock clock;

	std::vector<Entity*> entities;
	std::vector<Button*> main_menu;
	std::vector<Button*> pause_menu;

	// Init game before main loop

	game_running = true;
	current_state = Running;

	entities.reserve(2);

	// Game font init
	sf::Font font;
	if (!font.loadFromFile("resources/font/Spring is Coming - TTF.ttf"))
	{
		std::cout << "Couldn't load a game font." << std::endl;
	}

	// Main menu elements init
	sf::Text play_txt;
	play_txt.setFont(font);
	play_txt.setCharacterSize(34);
	play_txt.setString("Play!");
	
	// Background init
	sf::Texture ground_texture;
	if (!ground_texture.loadFromFile("resources/textures/grass_background.png"))
	{
		std::cout << "Couldn't load grass texture from file." << std::endl;
	}
	sf::Sprite ground_sprite;
	ground_sprite.setTexture(ground_texture);
	
	entities.push_back(new Entity(0, 0, 1280, 720, ground_sprite, NULL));

	// Player init
	sf::Texture player_texture;
	if (!player_texture.loadFromFile("resources/textures/mario_player.png"))
	{
		std::cout << "Couldn't load player texture from file." << std::endl;
	}
	sf::Sprite player_sprite;
	player_sprite.setTexture(player_texture);

	entities.push_back(new Entity(400, 350, 186, 312, player_sprite, player_update));

	// Main game loop

	while (window.isOpen() && game_running)
	{
		float time = clock.getElapsedTime().asMilliseconds();
		clock.restart();
		
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				destroy(&window);
		}

		if (current_state == Running)
		{
			update(&entities, time);
			render_update(&window, &entities);
		}
		else if (current_state == Menu)
		{
			update(&main_menu);
			render_text(&window, "This is a test!", font);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			if (current_state == Running)
				current_state = Pause;
			else if (current_state == Pause)
				current_state = Running;
			else
				game_running = false;
		}
	}

	destroy(&window);

	return 0;
}

void update(std::vector<Entity*>* entities, float t)
{
	for (Entity* entity : *entities)
	{
		if (entity->update != NULL && entity->active)
			entity->update(entity, t);
	}
}

void update(std::vector<Button*>* buttons)
{
	for (Button* button : *buttons)
	{

	}
}

void destroy(sf::RenderWindow* window)
{
	window->close();
}