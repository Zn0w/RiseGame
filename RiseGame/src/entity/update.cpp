#include "update.h"

void player_update(Entity* self, std::vector<Entity*>* entities, float t)
{
	// Keyboard input handling
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		self->dimensions.x -= 1.0f * t;
		self->sprite.setPosition(self->dimensions.x, self->dimensions.y);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		self->dimensions.x += 1.0f * t;
		self->sprite.setPosition(self->dimensions.x, self->dimensions.y);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		self->dimensions.y -= 1.0f * t;
		self->sprite.setPosition(self->dimensions.x, self->dimensions.y);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		self->dimensions.y += 1.0f * t;
		self->sprite.setPosition(self->dimensions.x, self->dimensions.y);
	}

	for (Entity* entity : *entities)
	{
		if (entity->type == Ground_grass && are_colliding(self->dimensions, entity->dimensions))
			std::cout << "Player and backround are colliding!" << std::endl;
	}
}