#pragma once

#include <vector>

#include "GLFW/glfw3.h"

#include "../entities/Entity.h"

// Initializes an opengl and sets glOrigin to the top left corner
void init(GLFWwindow* window);

// Clears screen from previous frame
void clearScreen();

// Renders game entities
void render(std::vector<Entity> entities);