#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

struct Entity
{
	int x, y, width, height;
	sf::Sprite sprite;
	bool active;
	void(*update) (Entity* self) = NULL;

	Entity(int xpos, int ypos, int w, int h, sf::Sprite s, void(*update_func)(Entity* self))
		: x(xpos), y(ypos), width(w), height(h), sprite(s), active(true), update(update_func)
	{	
		sprite.setPosition(x, y);
		sprite.setScale(
			(double) ((double) width / sprite.getTexture()->getSize().x), 
			(double) ((double) height / sprite.getTexture()->getSize().y)
		);

		//active = true;

		/*if (update_func == NULL)
		{
			std::cout << "Given update function = null" << std::endl;

			void raw_update();
			update = raw_update();
		}*/

		std::cout << "Sprite scale: " << sprite.getScale().x << " | " << sprite.getScale().y << std::endl;
	}
};