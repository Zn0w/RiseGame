#include <vector>
#include <iostream>

#include <GLFW/glfw3.h>

#include "rendering\rendering.h"
#include "entities/player.h"

void init();
void draw();
void update();

void glfwWindowResize(GLFWwindow* window, int width, int height);

std::vector<Entity*> entities;
GLFWwindow* window;

int main()
{
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
	glfwSetWindowSizeCallback(window, glfwWindowResize);

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
			std::cout << "FPS: " << frame_count << std::endl;
			frame_count = 0;
			previous_time += 1.0;
		}
		
		//int width, height;
		//glfwGetFramebufferSize(window, &width, &height);
		//glViewport(0, 0, width, height);
		
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

void glfwWindowResize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}