#include <vector>
#include <iostream>

#include <SFML/Audio.hpp>

#include "graphics/render.h"

#include "game_state.h"

#include "entity/entity.h"
#include "entity/update.h"

#include "button/button.h"
#include "button/events.h"

bool game_running = false;
GameState current_state;

void menu_update(std::vector<Button*>*); // Update function for Main and Pause state

void destroy(sf::RenderWindow*);


int main()
{
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Rise Game", sf::Style::Fullscreen);
	window.setVerticalSyncEnabled(true);
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

	sf::Vector2u window_size = window.getSize();


	// Main menu elements init

	// Play button
	sf::Text play_txt;
	play_txt.setFont(font);
	play_txt.setCharacterSize(34);
	play_txt.setString("Play!");
	main_menu.push_back(new Button(
		
		window_size.x / 16, 
		window_size.y / 9 * 6, 
		play_action, 
		play_txt, 
		true

	));

	// Guide button
	sf::Text guide_txt;
	guide_txt.setFont(font);
	guide_txt.setCharacterSize(34);
	guide_txt.setString("See guide");
	main_menu.push_back(new Button(
		
		window_size.x / 16, 
		(window_size.y / 9 * 6) + (window_size.y / 18), 
		guide_action, 
		guide_txt, 
		false

	));

	// Quit button
	sf::Text quit_txt;
	quit_txt.setFont(font);
	quit_txt.setCharacterSize(34);
	quit_txt.setString("Quit");
	main_menu.push_back(new Button(
		
		window_size.x / 16, 
		(window_size.y / 9 * 6) + (window_size.y / 18 * 2), 
		quit_action, 
		quit_txt, 
		false

	));


	// Pause menu elements init

	// Go back to the game button
	sf::Text back_txt;
	back_txt.setFont(font);
	back_txt.setCharacterSize(34);
	back_txt.setString("Go back to the game");
	pause_menu.push_back(new Button(

		window_size.x / 16,
		window_size.y / 9 * 6,
		play_action, 
		back_txt, 
		true

	));

	// Save button
	sf::Text save_txt;
	save_txt.setFont(font);
	save_txt.setCharacterSize(34);
	save_txt.setString("Save the game");
	pause_menu.push_back(new Button(
		
		window_size.x / 16,
		(window_size.y / 9 * 6) + (window_size.y / 18),
		save_action, 
		save_txt, 
		false
	
	));

	// Go to the main menu button
	sf::Text go_main_txt;
	go_main_txt.setFont(font);
	go_main_txt.setCharacterSize(34);
	go_main_txt.setString("Go to the main menu");
	pause_menu.push_back(new Button(
		
		window_size.x / 16,
		(window_size.y / 9 * 6) + (window_size.y / 18 * 2),
		main_menu_action, 
		go_main_txt, 
		false
	
	));


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

	entities.push_back(new Entity(Dimensions(0, 0, 1280, 720), Decor_object, ground_sprite, NULL));

	// Player init
	sf::Texture player_texture;
	if (!player_texture.loadFromFile("resources/textures/mario_player.png"))
	{
		std::cout << "Couldn't load player texture from file." << std::endl;
	}
	sf::Sprite player_sprite;
	player_sprite.setTexture(player_texture);

	entities.push_back(new Entity(Dimensions(400, 350, 186, 312), Player, player_sprite, player_update));


	// Doesn't work
	
	// Sound effects init

	// Test sound
	//sf::Sound buffer;
	//if (!sound_buffer.loadFromFile("laser.wav"))
		//std::cout << "Couldn't load laser sound effect." << std::endl;
	//sf::Sound laser_sound;
	//laser_sound.setBuffer(sound_buffer);


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
				if (entity->active && entity->type == Player)
				{
					// Keyboard input handling
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
					{
						entity->dimensions.x -= 1.0f * time;
						entity->sprite.setPosition(entity->dimensions.x, entity->dimensions.y);
					}

					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
					{
						entity->dimensions.x += 1.0f * time;
						entity->sprite.setPosition(entity->dimensions.x, entity->dimensions.y);
					}

					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
					{
						entity->dimensions.y -= 1.0f * time;
						entity->sprite.setPosition(entity->dimensions.x, entity->dimensions.y);
					}

					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
					{
						entity->dimensions.y += 1.0f * time;
						entity->sprite.setPosition(entity->dimensions.x, entity->dimensions.y);
					}

					for (Entity* other_entity : entities)
					{
						if (other_entity->type == Decor_object && areColliding(entity->dimensions, other_entity->dimensions))
						{
							// Doesn't work
							//laser_sound.play();
							//laser_sound.stop();
						}
					}
				}
			}
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
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
			{
				// Wait for player to release a Return key
				while (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
				{

				}

				current_state = Main;
			}
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