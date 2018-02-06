#include "rendering.h"

namespace rendering
{
	// Local for rendering.cpp file function, renders a text on the screen
	//void drawString(int xpos, int ypos, const char* text);
	
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
		if (entity.type == Gui_label)
		{
			//drawString(entity.x, entity.y, (Label) entity.text);
		}
		else
		{
			glBegin(GL_QUADS);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex2i(entity.x, entity.y);
			glVertex2i(entity.x + entity.width, entity.y);
			glVertex2i(entity.x + entity.width, entity.y + entity.height);
			glVertex2i(entity.x, entity.y + entity.height);
			glEnd();
		}
	}

	/*void drawString(int xpos, int ypos, const char* text)
	{

	}*/
}