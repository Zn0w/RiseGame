#pragma once

#include <SFML/Graphics.hpp>

struct Entity
{
	int x, y, width, height;
	sf::CircleShape shape;

	Entity(sf::CircleShape s)
		: shape(s)
	{
		shape.setFillColor(sf::Color::Green);
	}
};