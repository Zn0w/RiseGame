#pragma once

#include <stdint.h>

#include "keycodes.h"


struct KeyState
{
	uint8_t is_down : 4;
	uint8_t was_down : 4;
};

struct KeyboardInputMap
{
	KeyState keys[RG_MAX_KEYS];
};