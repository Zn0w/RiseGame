#include <vector>
#include <iostream>

#include "graphics/render.h"
#include "entity/entity.h"

bool game_running = false;

void update(std::vector<Entity>*);
void destroy();

int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!");
	std::vector<Entity> entities;

	// Init game before main loop
	sf::Texture ground_texture;
	if (!ground_texture.loadFromFile("D:/dev/RiseGame/RiseGame/src/resources/grass_background.png"))
	{
		std::cout << "Couldn't load grass texture from file." << std::endl;
	}
	sf::Sprite ground_sprite;
	ground_sprite.setTexture(ground_texture);
	
	entities.push_back(Entity(0, 0, 500, 500, ground_sprite));

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

void update(std::vector<Entity>* entities)
{
	
}

void destroy()
{

}