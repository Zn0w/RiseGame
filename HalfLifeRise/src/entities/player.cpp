#include "player.h"

namespace player
{
	bool top(Entity player, Entity otherEntity);
	
	void player::update(Entity* player, GLFWwindow* window, std::vector<Entity*> entities, double delta)
	{
		// "Gravity"
		player->y += 50 * delta;
		
		// Player entity input handling
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			player->x += 100 * delta;
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			player->x -= 100 * delta;
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			player->y -= 100 * delta;
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			player->y += 100 * delta;
		
		for (Entity* entity : entities)
		{
			if (entity->type == Object && collision::isColliding(player, entity))
				std::cout << "Player is hitting static object\n";
			else if (entity->type == Ground && collision::isCollidingBottom(player, entity))
				player->y -= 1;
		}
	}
}