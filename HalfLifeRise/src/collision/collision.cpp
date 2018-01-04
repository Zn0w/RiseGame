#include "collision.h"

namespace collision
{
	bool isColliding(Entity* e1, Entity* e2)
	{
		return e1->x + e1->width >= e2->x && e1->x <= e2->x + e2->width &&
			e1->y + e1->height >= e2->y && e1->y <= e2->y + e2->height;
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