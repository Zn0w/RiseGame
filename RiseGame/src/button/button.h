#pragma once

#include <SFML/Graphics.hpp>

struct Button
{
	int x, y;
	sf::Text text;
	void (*action) (void);
	bool selected = false;

	Button(int xpos, int ypos, void(*if_clicked) (void), sf::Text t)
		: x(xpos), y(ypos), action(if_clicked), text(t)
	{
		
	}
};