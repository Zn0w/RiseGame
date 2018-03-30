#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>

#include <SFML/Graphics.hpp>

struct Entity_data
{
	std::string type;
	std::string texture_path;
	float x, y, w, h;

	Entity_data(std::string t, std::string tex_p, float xpos, float ypos, float width, float height)
		: type(t), texture_path(tex_p), x(xpos), y(ypos), w(width), h(height)
	{

	}
};

std::map<std::string, sf::Texture> load_textures(std::string);

std::vector<Entity_data> load_entities_data(const char*);

void save_entities_data(std::vector<Entity_data>);