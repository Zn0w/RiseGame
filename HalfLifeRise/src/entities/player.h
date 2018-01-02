#pragma once

#include <iostream>

#include <GLFW/glfw3.h>

#include "entity.h"

namespace player
{
	// Input handling, collision detection of player
	void update(Entity* player, GLFWwindow* window, Entity wall);
}