#include "player.h"

namespace player
{
	bool top(Entity player, Entity otherEntity);
	
	void player::update(Entity* player, GLFWwindow* window, Entity wall)
	{
		// Player entity input handling
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			player->x += 1;
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			player->x -= 1;
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && !top(*player, wall))
			player->y -= 1;
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			player->y += 1;
		/*	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && player->ground_pos == player->y + player->height)
		std::cout << "Key E is pressed!\n";
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		std::cout << "Key E is pressed!\n";
		if (top(*player, wall))
			std::cout << "Top!";*/
	}

	bool top(Entity player, Entity otherEntity)
	{
		return player.y <= otherEntity.y + otherEntity.height &&
			player.y + player.height >= otherEntity.y + otherEntity.height &&
			player.x + player.width >= otherEntity.x &&
			player.x <= otherEntity.x + otherEntity.width;
	}
}

/*void col()
{
	if (collisionSide == TOP)
		return thisEntity->getY() <= otherEntity->getY() + otherEntity->getHeight() &&
		thisEntity->getY() + thisEntity->getHeight() >= otherEntity->getY() + otherEntity->getHeight() &&
		thisEntity->getX() + thisEntity->getWidth() >= otherEntity->getX() &&
		thisEntity->getX() <= otherEntity->getX() + otherEntity->getWidth();
	else if (collisionSide == BOTTOM)
		return thisEntity->getY() + thisEntity->getHeight() <= otherEntity->getY() + otherEntity->getHeight() &&
		thisEntity->getY() + thisEntity->getHeight() >= otherEntity->getY() &&
		thisEntity->getY() < otherEntity->getY() &&
		thisEntity->getX() + thisEntity->getWidth() >= otherEntity->getX() &&
		thisEntity->getX() <= otherEntity->getX() + otherEntity->getWidth();
	else if (collisionSide == LEFT)
		return thisEntity->getX() >= otherEntity->getX() &&
		thisEntity->getX() <= otherEntity->getX() + otherEntity->getWidth() &&
		thisEntity->getY() + thisEntity->getHeight() >= otherEntity->getY() &&
		thisEntity->getY() <= otherEntity->getY() + otherEntity->getHeight();
	else if (collisionSide == RIGHT)
		return thisEntity->getX() + thisEntity->getWidth() >= otherEntity->getX() &&
		thisEntity->getX() + thisEntity->getWidth() <= otherEntity->getX() + otherEntity->getWidth() &&
		thisEntity->getX() < otherEntity->getX() &&
		thisEntity->getY() + thisEntity->getHeight() >= otherEntity->getY() &&
		thisEntity->getY() <= otherEntity->getY() + otherEntity->getHeight();
}*/