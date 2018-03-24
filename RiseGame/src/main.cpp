#include <vector>
#include <iostream>

#include "graphics/render.h"

#include "game_state.h"

#include "entity/entity.h"
#include "entity/update.h"

#include "button/button.h"
#include "button/events.h"

bool game_running = false;
GameState current_state;

void running_update(std::vector<Entity*>*, float); // Update function for Running state
void menu_update(std::vector<Button*>*); // Update function for Main and Pause state
void guide_update(); // Update fucntion for Guide state

void destroy(sf::RenderWindow*);


int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!");
	window.setFramerateLimit(60);
	sf::Clock clock;

	std::vector<Entity*> entities;
	std::vector<Button*> main_menu;
	std::vector<Button*> pause_menu;

	// Init game before main loop

	game_running = true;
	current_state = Main;

	entities.reserve(2);


	// Game font init
	sf::Font font;
	if (!font.loadFromFile("resources/font/Spring is Coming - TTF.ttf"))
	{
		std::cout << "Couldn't load a game font." << std::endl;
	}


	// Main menu elements init

	// Play button
	sf::Text play_txt;
	play_txt.setFont(font);
	play_txt.setCharacterSize(34);
	play_txt.setString("Play!");
	main_menu.push_back(new Button(500, 250, play_action, play_txt, true));

	// Guide button
	sf::Text guide_txt;
	guide_txt.setFont(font);
	guide_txt.setCharacterSize(34);
	guide_txt.setString("See guide");
	main_menu.push_back(new Button(500, 350, guide_action, guide_txt, false));

	// Quit button
	sf::Text quit_txt;
	quit_txt.setFont(font);
	quit_txt.setCharacterSize(34);
	quit_txt.setString("Quit");
	main_menu.push_back(new Button(500, 450, quit_action, quit_txt, false));


	// Pause menu elements init

	// Go back to the game button
	sf::Text back_txt;
	back_txt.setFont(font);
	back_txt.setCharacterSize(34);
	back_txt.setString("Go back to the game");
	pause_menu.push_back(new Button(500, 250, play_action, back_txt, true));

	// Save button
	sf::Text save_txt;
	save_txt.setFont(font);
	save_txt.setCharacterSize(34);
	save_txt.setString("Save the game");
	pause_menu.push_back(new Button(500, 350, save_action, save_txt, false));

	// Go to the main menu button
	sf::Text go_main_txt;
	go_main_txt.setFont(font);
	go_main_txt.setCharacterSize(34);
	go_main_txt.setString("Go to the main menu");
	pause_menu.push_back(new Button(500, 450, main_menu_action, go_main_txt, false));


	// Guide window assets init
	sf::Text guide_test;
	guide_test.setFont(font);
	guide_test.setCharacterSize(34);
	guide_test.setString("This is guide. Press return to go back to the main menu.");


	// Game entities init
	
	// Background init
	sf::Texture ground_texture;
	if (!ground_texture.loadFromFile("resources/textures/grass_background.png"))
	{
		std::cout << "Couldn't load grass texture from file." << std::endl;
	}
	sf::Sprite ground_sprite;
	ground_sprite.setTexture(ground_texture);
	
	entities.push_back(new Entity(0, 0, 1280, 720, ground_sprite, NULL));

	// Player init
	sf::Texture player_texture;
	if (!player_texture.loadFromFile("resources/textures/mario_player.png"))
	{
		std::cout << "Couldn't load player texture from file." << std::endl;
	}
	sf::Sprite player_sprite;
	player_sprite.setTexture(player_texture);

	entities.push_back(new Entity(400, 350, 186, 312, player_sprite, player_update));


	// Main game loop

	while (window.isOpen() && game_running)
	{
		float time = clock.getElapsedTime().asMilliseconds();
		clock.restart();
		
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				destroy(&window);
		}

		// Find out what current game state is
		
		if (current_state == Running)
		{
			running_update(&entities, time);
			render_update(&window, &entities);
		}
		else if (current_state == Main)
		{
			menu_update(&main_menu);
			render_menu(&window, &main_menu);
		}
		else if (current_state == Pause)
		{
			menu_update(&pause_menu);
			render_menu(&window, &pause_menu);
		}
		else if (current_state == Saving)
		{
			// Save current game state
		}
		else if (current_state == Guide)
		{
			guide_update();
			render_text(&window, guide_test);
		}
		else if (current_state == Quit)
		{
			game_running = false;
			// Todo: ask user wether they want to save the game or not
		}
	}

	destroy(&window);

	return 0;
}

void running_update(std::vector<Entity*>* entities, float t)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		current_state = Pause;
	}
	
	for (Entity* entity : *entities)
	{
		if (entity->update != NULL && entity->active)
			entity->update(entity, t);
	}
}

void menu_update(std::vector<Button*>* buttons)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		// Wait for player to release a Return key
		while (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
		{

		}
		
		for (Button* button : *buttons)
		{
			if (button->selected)
			{
				button->action(&current_state);
				break;
			}
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		// Wait for player to release an Up key
		while (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{

		}

		for (int i = 0; i < buttons->size(); i++)
		{
			if (buttons->at(i)->selected && i > 0)
			{
				buttons->at(i)->selected = false;
				buttons->at(i)->text.setFillColor(sf::Color::White);
				buttons->at(i - 1)->selected = true;
				buttons->at(i - 1)->text.setFillColor(sf::Color::Green);
				break;
			}
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		// Wait for player to release a Down key
		while (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{

		}

		for (int i = 0; i < buttons->size(); i++)
		{
			if (buttons->at(i)->selected && i < buttons->size() - 1)
			{
				buttons->at(i)->selected = false;
				buttons->at(i)->text.setFillColor(sf::Color::White);
				buttons->at(i + 1)->selected = true;
				buttons->at(i + 1)->text.setFillColor(sf::Color::Green);
				break;
			}
		}
	}
}

void guide_update()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		// Wait for player to release a Return key
		while (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
		{

		}

		current_state = Main;
	}
}


void destroy(sf::RenderWindow* window)
{
	window->close();
}