#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

struct Entity
{
	float x, y, width, height;
	sf::Sprite sprite;
	bool active;
	void(*update) (Entity* self, std::vector<Entity*>* entities, float t) = NULL;

	Entity(float xpos, float ypos, float w, float h, sf::Sprite s, void(*update_func)(Entity* self, std::vector<Entity*>* entities, float t))
		: x(xpos), y(ypos), width(w), height(h), sprite(s), active(true), update(update_func)
	{	
		sprite.setPosition(x, y);
		sprite.setScale(
			(double) ((double) width / sprite.getTexture()->getSize().x), 
			(double) ((double) height / sprite.getTexture()->getSize().y)
		);

		std::cout << "Sprite scale: " << sprite.getScale().x << " | " << sprite.getScale().y << std::endl;
	}
};