#pragma once

#include <SFML/Graphics.hpp>

#include "../game_state.h"

struct Button
{
	int x, y;
	sf::Text text;
	void(*action) (GameState*) = NULL;
	bool selected;

	Button(int xpos, int ypos, void(*if_clicked) (GameState*), sf::Text t, bool is_selected)
		: x(xpos), y(ypos), action(if_clicked), text(t), selected(is_selected)
	{
		text.setPosition(x, y);
		
		if (selected)
			text.setFillColor(sf::Color::Green);
	}
};