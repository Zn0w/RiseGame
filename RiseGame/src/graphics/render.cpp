#include "render.h"

void render_update(sf::RenderWindow* window, std::vector<Entity*>* entities)
{
	window->clear();
	// Todo: don't render if entity is not within camera region
	for (Entity* entity : *entities)
	{
		if (entity->active)
			window->draw(entity->sprite);
	}
	window->display();
}

void render_menu(sf::RenderWindow* window, std::vector<Button*>* buttons)
{
	window->clear();

	for (Button* button : *buttons)
	{
		window->draw(button->text);
	}

	window->display();
}