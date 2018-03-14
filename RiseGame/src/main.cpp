#include <vector>

#include "graphics/render.h"
#include "entity/entity.h"

bool game_running = false;

std::vector<Entity> entities;

void update();
void destroy();

int main()
{
	// Init game before main loop
	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	sf::CircleShape shape(100.f);
	//shape.setFillColor(sf::Color::Green);

	entities.push_back(Entity(shape));

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// Will take vector<Entity> as a parameter
		update();

		render_update(&window, &entities);

		// Will be in render_update(window*, vector<Entity>)
		//window.clear();
		//window.draw(shape);
		//window.display();
	}

	return 0;
}

void update()
{
	
}

void destroy()
{

}