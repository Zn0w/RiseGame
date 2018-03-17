#include "update.h"

void player_update(Entity* entity)
{
	// Keyboard input handling
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