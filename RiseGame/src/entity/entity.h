#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

#include "../console/console.h"

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
	
	Ground_grass,

	Test_object,

	Decor_small_object
};

struct Entity
{
	Dimensions dimensions;
	EntityType type;
	sf::Sprite sprite;
	bool active;
	void(*update) (Entity* self, std::vector<Entity>* entities, float t) = NULL;

	Entity(Dimensions d, EntityType t, sf::Sprite s, void(*update_func)(Entity* self, std::vector<Entity>* entities, float t))
		: dimensions(d), type(t), sprite(s), active(true), update(update_func)
	{	
		sprite.setPosition(dimensions.x, dimensions.y);
		sprite.setScale(
			dimensions.width / sprite.getTexture()->getSize().x,
			dimensions.height / sprite.getTexture()->getSize().y
		);
		
		//console::print("Sprite scale: " + std::to_string(sprite.getScale().x) + " | " + std::to_string(sprite.getScale().y));
		std::cout << "Sprite scale: " << sprite.getScale().x << " | " << sprite.getScale().y << std::endl;
	}
};