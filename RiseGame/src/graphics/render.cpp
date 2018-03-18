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

void render_text(sf::RenderWindow* window, const char* text, sf::Font font)
{
	window->clear();

	sf::Text test_text;
	test_text.setFont(font);
	test_text.setCharacterSize(40);
	test_text.setString(text);
	test_text.setPosition(640, 300);

	window->draw(test_text);

	window->display();
}