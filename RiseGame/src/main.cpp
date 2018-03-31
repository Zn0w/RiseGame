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

#include "console/console.h"

bool game_running = false;
GameState current_state;

bool show_console = false;

void menu_update(std::vector<Button*>*); // Update function for Main and Pause state

void destroy(sf::RenderWindow*);


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


	// Console text init
	console::console_log.setFont(font);
	console::console_log.setCharacterSize(28);
	console::console_log.setFillColor(sf::Color::Blue);
	console::console_log.setPosition(window_size.x / 16 * 2, window_size.y / 9);
	console::console_log.setString("Testing");

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
	text_buffer.setString("This is guide. Press return to go back to the main menu. \n Press ctrl + D to go to the dev mode.");


	// Game entities init


	std::vector<Entity> entities;


	//sf::Sprite sprite_buffer;

	std::map<std::string, sf::Texture> textures = load_textures("resources/levels/level_1/textures.txt");
	std::vector<Entity_data> entity_data = load_entities_data("resources/levels/level_1/entities.txt");

	/*
		resources/levels/level_1/textures.txt contents:
			
			grass_background.png
			mario_player.png
			red_light.png
			flower_1.png
			flower_2.png
	*/

	/*
		resources/levels/level_1/entities.txt contents:
			
			Ground_grass;grass_background.png;0.0f;0.0f;1280.0f;720.0f;
			Decor_small_object;flower_1.png;200;200;50;50;
			Decor_small_object;flower_1.png;900;300;65;65;
			Decor_small_object;flower_1.png;500;500;50;50;
			Decor_small_object;flower_2.png;750;1000;80;80;
			Decor_small_object;flower_2.png;300;650;80;80;
			Test_object;red_light.png;1500.0f;800.0f;300.0f;300.0f;
			Player;mario_player.png;450.0f;500.0f;186.0f;312.0f;
	*/

	/*
		Those two files are the text description of the level 1.
		textures.txt scpecifies names of all the textures used in the level.
		entities.txt specifies all of the entities in the level: their type, texture path, coordinates, dimensions.
		The data in those files is processed by save_load_gamedata.h (the name will be changed) in order to load
		all of those entities and textures. Which is done with the code below.
	*/

	// Vector usage optimization
	entities.reserve(entity_data.size());

	for (Entity_data data : entity_data)
	{
		std::cout << data.type << std::endl;
		std::cout << data.texture_path << std::endl;
		std::cout << data.x << std::endl;
		std::cout << data.y << std::endl;
		std::cout << data.w << std::endl;
		std::cout << data.h << std::endl;
		
		sf::Sprite entity_sprite;

		// if texture wasn't loaded, then load error texture

		if (textures.at(data.texture_path).getSize().x == 0)
		{
			sf::Texture error_texture;
			error_texture.create(100, 100);
			entity_sprite.setTexture(error_texture);
			entity_sprite.setColor(sf::Color::Green);
		}
		else
			entity_sprite.setTexture(textures.at(data.texture_path));

		if (data.type == "Ground_grass")
		{
			std::cout << data.w / 2 << std::endl;
			entities.emplace_back(Entity(Dimensions(data.x, data.y, data.w, data.h), Ground_grass, entity_sprite, NULL));
		}
		else if (data.type == "Player")
		{
			entities.emplace_back(Entity(Dimensions(data.x, data.y, data.w, data.h), Player, entity_sprite, player_update));
		}
		else if (data.type == "Test_object")
		{
			entities.emplace_back(Entity(Dimensions(data.x, data.y, data.w, data.h), Test_object, entity_sprite, NULL));
		}
		else if (data.type == "Decor_small_object")
		{
			entities.emplace_back(Entity(Dimensions(data.x, data.y, data.w, data.h), Decor_small_object, entity_sprite, NULL));
		}
	}


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
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tilde))
			{
				show_console = true;
			}
			else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Tilde) && show_console)
			{
				current_state = Console;
			}

			for (int i = 0; i < entities.size(); i++)
			{
				Entity* entity = &entities.at(i);
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
		else if (current_state == Console)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tilde))
			{
				while (sf::Keyboard::isKeyPressed(sf::Keyboard::Tilde))
				{

				}
				
				show_console = false;
				current_state = Running;
			}

			render_text(&window, console::console_log);
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