#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

enum EntityType
{
	Player,
	Decor_Object
};

struct Entity
{
	int x, y, width, height;
	sf::Sprite sprite;
	EntityType type;

	Entity(int xpos, int ypos, int w, int h, sf::Sprite s, EntityType t)
		: x(xpos), y(ypos), width(w), height(h), sprite(s), type(t)
	{	
		sprite.setPosition(x, y);
		
		sprite.setScale(
			(double) ((double) width / sprite.getTexture()->getSize().x), 
			(double) ((double) height / sprite.getTexture()->getSize().y)
		);

		std::cout << "Sprite scale: " << sprite.getScale().x << " | " << sprite.getScale().y << std::endl;
	}
};