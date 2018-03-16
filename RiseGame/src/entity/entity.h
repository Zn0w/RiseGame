#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

struct Entity
{
	int x, y, width, height;
	sf::Sprite sprite;

	Entity(int xpos, int ypos, int w, int h, sf::Sprite s)
		: x(xpos), y(ypos), width(w), height(h), sprite(s)
	{	
		sprite.setPosition(x, y);
		
		sprite.setScale(
			(double) ((double) width / sprite.getTexture()->getSize().x), 
			(double) ((double) height / sprite.getTexture()->getSize().y)
		);

		std::cout << "Sprite scale: " << sprite.getScale().x << " | " << sprite.getScale().y << std::endl;
	}
};