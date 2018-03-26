#include "update.h"

void player_update(Entity* entity, std::vector<Entity*>* entities, float t)
{
	// Keyboard input handling
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		entity->dimensions.x -= 1.0f * t;
		entity->sprite.setPosition(entity->dimensions.x, entity->dimensions.y);
		//entity->sprite.move(-0.001 * t, 0);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		entity->dimensions.x += 1.0f * t;
		entity->sprite.setPosition(entity->dimensions.x, entity->dimensions.y);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		entity->dimensions.y -= 1.0f * t;
		entity->sprite.setPosition(entity->dimensions.x, entity->dimensions.y);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		entity->dimensions.y += 1.0f * t;
		entity->sprite.setPosition(entity->dimensions.x, entity->dimensions.y);
	}
}