#pragma once

#include <SFML/Graphics.hpp>

struct Button
{
	int x, y;
	sf::Text text;
	void(*action) (void) = NULL;
	bool selected;

	Button(int xpos, int ypos, void(*if_clicked) (void), sf::Text t, bool is_selected)
		: x(xpos), y(ypos), action(if_clicked), text(t), selected(is_selected)
	{
		
	}
};