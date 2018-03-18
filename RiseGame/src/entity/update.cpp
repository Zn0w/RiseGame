#include "update.h"

void player_update(Entity* entity, float t)
{
	// Keyboard input handling
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		entity->x -= 1.0f * t;
		entity->sprite.setPosition(entity->x, entity->y);
		//entity->sprite.move(-0.001 * t, 0);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		entity->x += 1.0f * t;
		entity->sprite.setPosition(entity->x, entity->y);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		entity->y -= 1.0f * t;
		entity->sprite.setPosition(entity->x, entity->y);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		entity->y += 1.0f * t;
		entity->sprite.setPosition(entity->x, entity->y);
	}
}