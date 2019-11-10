#include "gamepad.h"


void LoadXInput()
{
	HMODULE XInputLibrary = LoadLibrary("xinput1_4.dll");
	// if version 1.4 of xinput is not supported by the os, load version 1.3
	// NOTE : made primarily to support windows 8, since windows 8 has only one version of xinput which is 1.4
	if (!XInputLibrary)
	{
		HMODULE XInputLibrary = LoadLibrary("xinput1_3.dll");
	}

	if (XInputLibrary)
	{
		XInputGetState = (xinput_get_state*)GetProcAddress(XInputLibrary, "XInputGetState");
		XInputSetState = (xinput_set_state*)GetProcAddress(XInputLibrary, "XInputSetState");
	}
	else
		OutputDebugStringA("Failed to load an XInput library\n");
}

void getGamepadInput(GamepadInputMap* gamepad_input)
{
	// get gamepad input
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	{
		XINPUT_STATE controller_state;
		//ZeroMemory(&state, sizeof(XINPUT_STATE));

		// Get the state of the controller from XInput
		if (XInputGetState(i, &controller_state) == ERROR_SUCCESS)	// Controller is connected
		{
			*gamepad_input = {};

			gamepad_input->buttons.up = (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
			gamepad_input->buttons.down = (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
			gamepad_input->buttons.left = (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
			gamepad_input->buttons.right = (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
			gamepad_input->buttons.start = (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_START);
			gamepad_input->buttons.back = (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK);
			gamepad_input->buttons.left_thumb = (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB);
			gamepad_input->buttons.right_thumb = (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB);
			gamepad_input->buttons.left_shoulder = (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
			gamepad_input->buttons.right_shoulder = (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
			gamepad_input->buttons.a_button = (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_A);
			gamepad_input->buttons.b_button = (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_B);
			gamepad_input->buttons.x_button = (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_X);
			gamepad_input->buttons.y_button = (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_Y);

			gamepad_input->left_trigger = controller_state.Gamepad.bLeftTrigger;
			gamepad_input->right_trigger = controller_state.Gamepad.bRightTrigger;

			gamepad_input->thumb_left_x = controller_state.Gamepad.sThumbLX;
			gamepad_input->thumb_left_y = controller_state.Gamepad.sThumbLY;
			gamepad_input->thumb_right_x = controller_state.Gamepad.sThumbRX;
			gamepad_input->thumb_right_y = controller_state.Gamepad.sThumbRY;
		}
		else	// Controller is not connected 
		{

		}
	}
}