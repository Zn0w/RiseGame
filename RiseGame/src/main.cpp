#include <vector>
#include <iostream>

#include "graphics/render.h"
#include "entity/entity.h"

bool game_running = false;

void update(std::vector<Entity*>*);
void destroy();

int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!");
	std::vector<Entity*> entities;

	// Init game before main loop
	
	// Background init
	sf::Texture ground_texture;
	if (!ground_texture.loadFromFile("resources/grass_background.png"))
	{
		std::cout << "Couldn't load grass texture from file." << std::endl;
	}
	sf::Sprite ground_sprite;
	ground_sprite.setTexture(ground_texture);
	
	entities.push_back(new Entity(0, 0, 1280, 720, ground_sprite, Decor_Object));

	// Player init
	sf::Texture player_texture;
	if (!player_texture.loadFromFile("resources/mario_player.png"))
	{
		std::cout << "Couldn't load player texture from file." << std::endl;
	}
	sf::Sprite player_sprite;
	player_sprite.setTexture(player_texture);

	entities.push_back(new Entity(400, 350, 186, 312, player_sprite, Player));

	// Main game loop

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		update(&entities);

		render_update(&window, &entities);
	}

	destroy();

	return 0;
}

void update(std::vector<Entity*>* entities)
{
	for (Entity* entity : *entities)
	{
		if (entity->type == Player)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				std::cout << "Left key is pressed!" << std::endl;
				entity->x -= 1;
				entity->sprite.setPosition(entity->x, entity->y);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				std::cout << "Right key is pressed!" << std::endl;
				entity->x += 1;
				entity->sprite.setPosition(entity->x, entity->y);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				std::cout << "Up key is pressed!" << std::endl;
				entity->y -= 1;
				entity->sprite.setPosition(entity->x, entity->y);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				std::cout << "Down key is pressed!" << std::endl;
				entity->y += 1;
				entity->sprite.setPosition(entity->x, entity->y);
			}
		}
		else if (entity->type == Decor_Object)
		{
			//std::cout << "Decor object update" << std::endl;
		}
	}
}

void destroy()
{

}