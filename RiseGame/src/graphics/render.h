#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "../entity/entity.h"

void render_update(sf::RenderWindow*, std::vector<Entity*>*);

void render_text(sf::RenderWindow*, const char*, sf::Font);