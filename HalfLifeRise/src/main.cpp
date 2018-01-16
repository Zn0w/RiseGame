#include <vector>
#include <iostream>

#include <GLFW/glfw3.h>

#include "rendering\rendering.h"
#include "entities/player.h"

void init();
void draw();
void update(double delta);

void glfwWindowResize(GLFWwindow* window, int width, int height);

void createEntity(int x, int y, int width, int height, bool active, EntityType type);
void deleteEntity(Entity* entity);

std::vector<Entity*> entities;
GLFWwindow* window;

int main()
{
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1600, 900, "Hello World", NULL, NULL);
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
	double current_time;
	double delta;

	while (!glfwWindowShouldClose(window))
	{	
		current_time = glfwGetTime();
		delta = current_time - previous_time;
		previous_time = current_time;
		
		update(delta);
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
	
	createEntity(50, 70, 80, 80, true, Player);

	createEntity(150, 40, 80, 40, true, Object);

	createEntity(50, 300, 200, 40, true, Ground);
}

void draw()
{
	rendering::clearScreen();
	for (Entity* entity : entities)
		if (entity->active)
			rendering::render(*entity);
}

void update(double delta)
{
	for (Entity* entity : entities)
	{
		if (entity->type == Player)
			player::update(entity, window, entities, delta);
	}
}

void glfwWindowResize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void createEntity(int x, int y, int width, int height, bool active, EntityType type)
{
	Entity* entity = new Entity(x, y, width, height, active, type);
	entities.push_back(entity);
}

void deleteEntity(Entity* entity)
{
	// to be implemented
}