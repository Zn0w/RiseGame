#pragma once

#include <iostream>
#include <vector>

#include <GLFW/glfw3.h>

#include "entity.h"
#include "../collision/collision.h"

namespace player
{
	// Input handling, collision detection of player
	void update(Entity* player, GLFWwindow* window, std::vector<Entity*> entities);
}