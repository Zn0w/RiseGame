#include <vector>
#include <iostream>

#include <SFML/Audio.hpp>

#include "graphics/render.h"

#include "game_state.h"

#include "entity/entity.h"
#include "entity/update.h"

#include "button/button.h"
#include "button/events.h"

#include "save_load_operations/save_load_gamedata.h"

bool game_running = false;
GameState current_state;

void menu_update(std::vector<Button*>*); // Update function for Main and Pause state

void destroy(sf::RenderWindow*);

sf::Texture load_texture(const char*);


int main()
{
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Rise Game", sf::Style::Fullscreen);
	window.setVerticalSyncEnabled(true);
	sf::Clock clock;

	// Init game before main loop

	game_running = true;
	current_state = Main;


	// Game font init
	sf::Font font;
	if (!font.loadFromFile("resources/font/Spring is Coming - TTF.ttf"))
	{
		std::cout << "Couldn't load a game font." << std::endl;
	}

	sf::Vector2u window_size = window.getSize();


	// Main menu elements init

	sf::Text text_buffer;
	text_buffer.setFont(font);
	text_buffer.setCharacterSize(34);

	std::vector<Button*> main_menu;

	// Play button
	text_buffer.setString("Play!");
	main_menu.push_back(new Button(
		
		window_size.x / 16, 
		window_size.y / 9 * 6, 
		play_action, 
		text_buffer,
		true

	));

	// Guide button
	text_buffer.setString("See guide");
	main_menu.push_back(new Button(
		
		window_size.x / 16, 
		(window_size.y / 9 * 6) + (window_size.y / 18), 
		guide_action, 
		text_buffer,
		false

	));

	// Settings button
	text_buffer.setString("Settings");
	main_menu.push_back(new Button(

		window_size.x / 16,
		(window_size.y / 9 * 6) + (window_size.y / 18 * 2),
		settings_action,
		text_buffer,
		false

	));

	// Quit button
	text_buffer.setString("Quit");
	main_menu.push_back(new Button(
		
		window_size.x / 16, 
		(window_size.y / 9 * 6) + (window_size.y / 18 * 3), 
		quit_action, 
		text_buffer,
		false

	));


	// Pause menu elements init

	std::vector<Button*> pause_menu;

	// Go back to the game button
	text_buffer.setString("Go back to the game");
	pause_menu.push_back(new Button(

		window_size.x / 16,
		window_size.y / 9 * 6,
		play_action, 
		text_buffer,
		true

	));

	// Save button
	text_buffer.setString("Save the game");
	pause_menu.push_back(new Button(
		
		window_size.x / 16,
		(window_size.y / 9 * 6) + (window_size.y / 18),
		save_action, 
		text_buffer,
		false
	
	));

	// Go to the main menu button
	text_buffer.setString("Go to the main menu");
	pause_menu.push_back(new Button(
		
		window_size.x / 16,
		(window_size.y / 9 * 6) + (window_size.y / 18 * 2),
		main_menu_action, 
		text_buffer,
		false
	
	));


	// Guide window assets init
	text_buffer.setString("This is guide. Press return to go back to the main menu.");


	// Game entities init


	std::vector<Entity*> entities;


	sf::Sprite sprite_buffer;

	std::vector<Entity_data> entity_data = load_entities_data("gamedata/level1.txt");
	for (Entity_data data : entity_data)
	{
		std::cout << data.type << std::endl;
	}


	// Background init
	sf::Texture background_texture;
	if (!background_texture.loadFromFile("resources/textures/grass_background.png"))
	{
		std::cout << "Couldn't load grass texture from file." << std::endl;
	}
	sprite_buffer.setTexture(background_texture);

	entities.push_back(new Entity(Dimensions(0, 0, 1280, 720), Ground_grass, sprite_buffer, NULL));

	// Player init
	sf::Texture player_texture;
	if (!player_texture.loadFromFile("resources/textures/mario_player.png"))
	{
		std::cout << "Couldn't load player texture from file." << std::endl;
	}
	sprite_buffer.setTexture(player_texture);

	entities.push_back(new Entity(Dimensions(400, 350, 186, 312), Player, sprite_buffer, player_update));

	// Collision test signal init
	sf::Texture signal_texture;
	if (!signal_texture.loadFromFile("resources/textures/red_light.png"))
	{
		std::cout << "Couldn't load red signal texture from file." << std::endl;
	}
	sprite_buffer.setTexture(signal_texture);

	entities.push_back(new Entity(Dimensions(1500, 800, 300, 300), Test_object, sprite_buffer, NULL));


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
			else if (event.type == sf::Event::Resized)
				std::cout << window.getSize().x << " | " << window.getSize().y << std::endl;
		}

		// Find out what current game state is

		if (current_state == Running)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				current_state = Pause;
			}

			for (Entity* entity : entities)
			{
				if (entity->update != NULL)
					entity->update(entity, &entities, time);
			}
			render_entities(&window, &entities);
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
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
			{
				// Wait for player to release a Return key
				while (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
				{

				}

				current_state = Main;
			}
			render_text(&window, text_buffer);
		}
		else if (current_state == Settings)
		{
			// Open settings menu
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

void destroy(sf::RenderWindow* window)
{
	window->close();
}

sf::Texture load_texture(const char* texture_path)
{
	sf::Texture texture;
	if (!texture.loadFromFile(texture_path))
	{
		
	}
	else
	{
		texture.create(0, 0);
	}
	return texture;
}