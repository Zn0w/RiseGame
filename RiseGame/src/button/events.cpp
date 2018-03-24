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
	*state = Quit;
}

void save_action(GameState* state)
{
	*state = Saving;
}

void main_menu_action(GameState* state)
{
	*state = Main;
}