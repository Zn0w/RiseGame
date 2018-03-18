#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "../entity/entity.h"

void render_update(sf::RenderWindow*, std::vector<Entity*>*); // Todo: don't render if entity is not within camera region

void render_text(sf::RenderWindow*, const char*, sf::Font);