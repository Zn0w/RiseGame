#pragma once

#include <string>

#include <SFML/Graphics.hpp>

namespace console
{
	static sf::Text console_log;
	
	void print(std::string);
}