#pragma once

#include <SFML/Graphics.hpp>

struct Entity
{
	int x, y, width, height;
	sf::Sprite sprite;

	Entity(int xpos, int ypos, int w, int h, sf::Sprite s)
		: x(xpos), y(ypos), width(w), height(h), sprite(s)
	{
		//sprite.setScale(1280 / 270, 720 / 270);
	}
};