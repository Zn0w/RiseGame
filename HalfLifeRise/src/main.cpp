#include <vector>
#include <iostream>

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
	std::vector<Entity*> entities;
	
	Entity* player = new Entity(50, 70, 80, 80);
	entities.push_back(player);

	init(window);
	while (!glfwWindowShouldClose(window))
	{
		// Player entity input handling
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			player->x += 1;
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			player->x -= 1;
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && player->ground_pos == player->y + player->height)
			std::cout << "Key E is pressed!\n";
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			std::cout << "Key E is pressed!\n";
		
		clearScreen();

		render(entities);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}