#pragma once

#include <string>
#include <vector>
#include <fstream>

struct Entity_data
{
	std::string type;
	std::string texture_path;
	float x, y, w, h;
};

std::vector<Entity_data> get_entities_data(const char* filepath);