#include <vector>
#include <iostream>

#include "graphics/render.h"
#include "entity/entity.h"
#include "entity/update.h"

enum GameState
{
	Menu,
	Pause,
	Running
};

bool game_running = false;
GameState current_state;

void update(std::vector<Entity*>*);
void destroy(sf::RenderWindow*);

int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!");
	std::vector<Entity*> entities;

	// Init game before main loop

	game_running = true;
	current_state = Menu;

	// Game font init
	sf::Font font;
	if (!font.loadFromFile("resources/font/Spring is Coming - TTF.ttf"))
	{
		std::cout << "Couldn't load a game font." << std::endl;
	}
	
	// Background init
	sf::Texture ground_texture;
	if (!ground_texture.loadFromFile("resources/textures/grass_background.png"))
	{
		std::cout << "Couldn't load grass texture from file." << std::endl;
	}
	sf::Sprite ground_sprite;
	ground_sprite.setTexture(ground_texture);
	
	entities.push_back(new Entity(0, 0, 1280, 720, ground_sprite, Decor_Object));

	// Player init
	sf::Texture player_texture;
	if (!player_texture.loadFromFile("resources/textures/mario_player.png"))
	{
		std::cout << "Couldn't load player texture from file." << std::endl;
	}
	sf::Sprite player_sprite;
	player_sprite.setTexture(player_texture);

	entities.push_back(new Entity(400, 350, 186, 312, player_sprite, Player));

	// Main game loop

	while (window.isOpen() || game_running)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				destroy(&window);
		}

		if (current_state == Running)
		{
			update(&entities);
			render_update(&window, &entities);
		}
		else if (current_state == Menu)
		{
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

void update(std::vector<Entity*>* entities)
{
	for (Entity* entity : *entities)
	{
		if (entity->type == Player)
		{
			player_update(entity);
		}
		else if (entity->type == Decor_Object)
		{
			//std::cout << "Decor object update" << std::endl;
		}
	}
}

void destroy(sf::RenderWindow* window)
{
	window->close();
}