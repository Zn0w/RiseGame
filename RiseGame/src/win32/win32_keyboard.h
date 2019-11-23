#pragma once

#include <windows.h>
#include <stdint.h>

#include "../game.h"

static KeyboardInputMap keyboard = {};


void getKeyboardInput(uint32_t VKCode, bool was_down, bool is_down)
{	
	if (was_down != is_down)
	{
		if (VKCode == 'W')
		{
			if (was_down && !is_down)
				OutputDebugStringA("W is released!\n");
		}
		else if (VKCode == 'A')
		{
	
		}
		else if (VKCode == 'S')
		{
	
		}
		else if (VKCode == 'D')
		{
	
		}
		else if (VKCode == 'Q')
		{
	
		}
		else if (VKCode == 'E')
		{
	
		}
		else if (VKCode == VK_UP)
		{
			keyboard.keys[RG_UP].is_down = is_down;
			keyboard.keys[RG_UP].was_down = was_down;
		}
		else if (VKCode == VK_DOWN)
		{
			keyboard.keys[RG_DOWN].is_down = is_down;
			keyboard.keys[RG_DOWN].was_down = was_down;
		}
		else if (VKCode == VK_LEFT)
		{
			keyboard.keys[RG_LEFT].is_down = is_down;
			keyboard.keys[RG_LEFT].was_down = was_down;
		}
		else if (VKCode == VK_RIGHT)
		{
			keyboard.keys[RG_RIGHT].is_down = is_down;
			keyboard.keys[RG_RIGHT].was_down = was_down;
		}
		else if (VKCode == VK_SPACE)
		{
	
		}
		else if (VKCode == VK_ESCAPE)
		{
	
		}
	}
}