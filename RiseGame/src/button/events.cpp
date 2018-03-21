#include "events.h"

void play_action(GameState* state)
{
	*state = Running;
}

void guide_action(GameState* state)
{
	*state = Guide;
}

void quit_action(GameState* state)
{
	std::cout << "Quit button is pressed!" << std::endl;
}