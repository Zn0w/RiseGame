#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

struct Dimensions
{
	float x, y, width, height;

	Dimensions(float xpos, float ypos, float w, float h)
		: x(xpos), y(ypos), width(w), height(h)
	{

	}
};

enum EntityType
{
	Player,
	Decor_object
};

struct Entity
{
	Dimensions dimensions;
	EntityType type;
	sf::Sprite sprite;
	bool active;
	void(*update) (Entity* self, std::vector<Entity*>* entities, float t) = NULL;

	Entity(Dimensions d, EntityType t, sf::Sprite s, void(*update_func)(Entity* self, std::vector<Entity*>* entities, float t))
		: dimensions(d), type(t), sprite(s), active(true), update(update_func)
	{	
		sprite.setPosition(dimensions.x, dimensions.y);
		sprite.setScale(
			(double) ((double) dimensions.width / sprite.getTexture()->getSize().x),
			(double) ((double) dimensions.height / sprite.getTexture()->getSize().y)
		);

		std::cout << "Sprite scale: " << sprite.getScale().x << " | " << sprite.getScale().y << std::endl;
	}
};