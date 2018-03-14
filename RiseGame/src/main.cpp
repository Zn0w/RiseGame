#include <vector>

#include "graphics/render.h"
#include "entity/entity.h"

bool game_running = false;

void update(std::vector<Entity>*);
void destroy();

int main()
{
	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	std::vector<Entity> entities;

	// Init game before main loop
	entities.push_back(Entity(sf::CircleShape(100.f)));

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		update(&entities);

		render_update(&window, &entities);
	}

	return 0;
}

void update(std::vector<Entity>* entities)
{
	
}

void destroy()
{

}