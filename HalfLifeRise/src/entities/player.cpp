#include "player.h"

namespace player
{
	bool top(Entity player, Entity otherEntity);
	
	void player::update(Entity* player, GLFWwindow* window, std::vector<Entity*> entities)
	{
		// Player entity input handling
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			player->x += 1;
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			player->x -= 1;
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			player->y -= 1;
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			player->y += 1;
		
		for (Entity* entity : entities)
			if (entity->type == Object && collision::isColliding(player, entity))
				std::cout << "Player is hitting static object\n";
	}
}