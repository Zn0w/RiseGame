#pragma once

#include "../entity.h"

enum ButtonType
{
	Start,
	Settings,
	Guide,
	Quit
};

void onClick(ButtonType type);