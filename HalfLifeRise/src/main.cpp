#include <vector>

#include <GLFW/glfw3.h>

#include "rendering\rendering.h"

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

	// Init a game world entities
	std::vector<Entity> entities;
	
	Entity player(50, 70, 80, 80);
	entities.push_back(player);

	init(window);
	while (!glfwWindowShouldClose(window))
	{
		clearScreen();

		render(entities);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}