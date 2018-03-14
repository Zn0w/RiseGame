#include "render.h"

void render_update(sf::RenderWindow* window, std::vector<Entity>* entities)
{
	window->clear();
	// Then loop over a vector array and window->draw(entity.texture);
	for (Entity entity : *entities)
	{
		window->draw(entity.sprite);
	}
	window->display();
}