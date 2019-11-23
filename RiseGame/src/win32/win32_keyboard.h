#pragma once

#include <windows.h>
#include <stdint.h>

#include "../game.h"

static KeyboardInputMap keyboard = {};


void getKeyboardInput(uint32_t VKCode, bool was_down, bool is_down)
{	
	if (was_down != is_down)
	{
		if (VKCode >= 'A' && VKCode <= 'Z')
		{
			uint8_t rg_keycode = VKCode - 'A';
			keyboard.keys[rg_keycode].is_down = is_down;
			keyboard.keys[rg_keycode].was_down = was_down;
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
		else if (VKCode >= '0' && VKCode <= '9')
		{
			uint8_t rg_keycode = VKCode - '0' + RG_0;
			keyboard.keys[rg_keycode].is_down = is_down;
			keyboard.keys[rg_keycode].was_down = was_down;
		}
		else if (VKCode == VK_OEM_MINUS)
		{
			keyboard.keys[RG_MINUS].is_down = is_down;
			keyboard.keys[RG_MINUS].was_down = was_down;
		}
		else if (VKCode == VK_OEM_PLUS)
		{
			keyboard.keys[RG_PLUS].is_down = is_down;
			keyboard.keys[RG_PLUS].was_down = was_down;
		}
		else if (VKCode == VK_SHIFT)
		{
			keyboard.keys[RG_SHIFT].is_down = is_down;
			keyboard.keys[RG_SHIFT].was_down = was_down;
		}
		else if (VKCode == VK_CONTROL)
		{
			keyboard.keys[RG_CONTROL].is_down = is_down;
			keyboard.keys[RG_CONTROL].was_down = was_down;
		}
		else if (VKCode == VK_MENU)
		{
			keyboard.keys[RG_ALT].is_down = is_down;
			keyboard.keys[RG_ALT].was_down = was_down;
		}
		else if (VKCode == VK_SPACE)
		{
			keyboard.keys[RG_SPACE].is_down = is_down;
			keyboard.keys[RG_SPACE].was_down = was_down;
		}
		else if (VKCode == VK_ESCAPE)
		{
			keyboard.keys[RG_ESCAPE].is_down = is_down;
			keyboard.keys[RG_ESCAPE].was_down = was_down;
		}
		else if (VKCode == VK_CAPITAL)
		{
			keyboard.keys[RG_CAPSLOCK].is_down = is_down;
			keyboard.keys[RG_CAPSLOCK].was_down = was_down;
		}
		else if (VKCode == VK_TAB)
		{
			keyboard.keys[RG_TAB].is_down = is_down;
			keyboard.keys[RG_TAB].was_down = was_down;
		}
		else if (VKCode == VK_RETURN)
		{
			keyboard.keys[RG_ENTER].is_down = is_down;
			keyboard.keys[RG_ENTER].was_down = was_down;
		}
		else if (VKCode == VK_BACK)
		{
			keyboard.keys[RG_BACKSPACE].is_down = is_down;
			keyboard.keys[RG_BACKSPACE].was_down = was_down;
		}
		else if (VKCode == VK_OEM_3)
		{
			keyboard.keys[RG_TILDE].is_down = is_down;
			keyboard.keys[RG_TILDE].was_down = was_down;
		}
	}
}