#include "render.h"

void render_update(sf::RenderWindow* window, std::vector<Entity*>* entities)
{
	window->clear();
	// Then loop over a vector array and window->draw(entity.texture);
	for (Entity* entity : *entities)
	{
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