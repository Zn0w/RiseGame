#include "collision.h"

namespace collision
{
	bool isColliding(Entity* e1, Entity* e2)
	{
		return e1->x + e1->width >= e2->x && e1->x <= e2->x + e2->width &&
			e1->y + e1->height >= e2->y && e1->y <= e2->y + e2->height;
	}

	bool isCollidingRight(Entity* e1, Entity* e2)
	{
		return e1->x + e1->width >= e2->x && e1->x + e1->width <= e2->x + e2->width &&
			e1->x < e2->x && e1->y + e1->height >= e2->y && e1->y <= e2->y + e2->height;
	}

	bool isCollidingLeft(Entity* e1, Entity* e2)
	{
		return e1->x >= e2->x && e1->x <= e2->x + e2->width &&
			e1->y + e1->height >= e2->y && e1->y <= e2->y + e2->height;
	}

	bool isCollidingTop(Entity* e1, Entity* e2)
	{
		return e1->y <= e2->y + e2->height && e1->y + e1->height >= e2->y + e2->height &&
			e1->x + e1->width >= e2->x && e1->x <= e2->x + e2->width;
	}

	bool isCollidingBottom(Entity* e1, Entity* e2)
	{
		return e1->y + e1->height <= e2->y + e2->height && e1->y + e1->height >= e2->y && e1->y < e2->y &&
			e1->x + e1->width >= e2->x && e1->x <= e2->x + e2->width;
	}
}