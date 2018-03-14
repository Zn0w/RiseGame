#pragma once

#include <SFML/Graphics.hpp>

struct Entity
{
	int x, y, width, height;
	sf::Sprite sprite;

	Entity(sf::Sprite s)
		: sprite(s)
	{
		
	}
};