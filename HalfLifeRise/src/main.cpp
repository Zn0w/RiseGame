#include <vector>
#include <iostream>

#include <GLFW/glfw3.h>

#include "rendering\rendering.h"
#include "entities/player.h"

void init();
void draw();
void update();

std::vector<Entity> entities;

int main()
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Game init
	rendering::init(window);
	init();

	while (!glfwWindowShouldClose(window))
	{	
		update();
		draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void init()
{
	Entity player(50, 70, 80, 80, true);
	entities.push_back(player);

	Entity wall(150, 40, 80, 40, true);
	entities.push_back(wall);
}

void draw()
{
	rendering::clearScreen();
	for (Entity entity : entities)
		if (entity.active)
			rendering::render(entity);
}

void update()
{
	//player update
}