#include "rendering.h"

void init(GLFWwindow* window)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	
	glMatrixMode(GL_PROJECTION);
	glOrtho(0, width, height, 0, 1, -1);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);
}

void clearScreen()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void render(Entity entity)
{
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2i(entity.x, entity.y);
	glVertex2i(entity.x + entity.width, entity.y);
	glVertex2i(entity.x + entity.width, entity.y + entity.height);
	glVertex2i(entity.x, entity.y + entity.height);
	glEnd();
}