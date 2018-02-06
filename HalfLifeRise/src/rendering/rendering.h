#pragma once

#include "GLFW/glfw3.h"

#include "../entities/entity.h"
#include "../entities/gui/label.h"

namespace rendering
{
	// Initializes an opengl and sets glOrigin to the top left corner
	void init(GLFWwindow* window);

	// Clears screen from previous frame
	void clearScreen();

	// Renders game entities
	void render(Entity entity);
}