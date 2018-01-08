#include <vector>
#include <iostream>

#include <GLFW/glfw3.h>

#include "rendering\rendering.h"
#include "entities/player.h"

void init();
void draw();
void update();

std::vector<Entity*> entities;
GLFWwindow* window;

int main()
{
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1280, 720, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Game init
	init();

	double previous_time = glfwGetTime();
	int frame_count = 0;

	while (!glfwWindowShouldClose(window))
	{	
		double current_time = glfwGetTime();
		frame_count++;
		if (current_time - previous_time >= 1.0)
		{
			std::cout << "FPS: " << /*1000.0 / */frame_count << std::endl;
			frame_count = 0;
			previous_time += 1.0;
		}
		
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
	rendering::init(window);
	
	Entity* player = new Entity(50, 70, 80, 80, true, Player);
	entities.push_back(player);

	Entity* wall = new Entity(150, 40, 80, 40, true, Object);
	entities.push_back(wall);
}

void draw()
{
	rendering::clearScreen();
	for (Entity* entity : entities)
		if (entity->active)
			rendering::render(*entity);
}

void update()
{
	for (Entity* entity : entities)
	{
		if (entity->type == Player)
			player::update(entity, window, entities);
	}
}