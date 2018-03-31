#pragma once

#include <string>

#include <SFML/Graphics.hpp>

namespace console
{
	extern sf::Text console_log;
	
	void print(std::string);
}