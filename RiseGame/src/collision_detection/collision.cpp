#include "collision.h"

bool areColliding(Dimensions e1, Dimensions e2)
{
	return e1.x + e1.width >= e2.x && e1.x <= e2.x + e2.width &&
		   e1.y + e1.height >= e2.y && e1.y <= e2.y + e2.height;
}