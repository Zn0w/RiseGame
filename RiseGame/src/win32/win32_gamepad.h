#pragma once

#include <windows.h>
#include <xinput.h>

#include "../game.h"


//-----------------------------------------------------------------------------------------------------------------
// Load XInput library (loading the dll)
typedef DWORD WINAPI xinput_get_state(DWORD dwUserIndex, XINPUT_STATE* pState);
typedef DWORD WINAPI xinput_set_state(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration);

// NOTE : stub functions (functions with no implementation), they will be used if xinput library failed to load
// because we want to be able to play the game even if we don't have a library to support gamepad input (use keyboard instead)
DWORD WINAPI xinput_get_state_stub(DWORD dwUserIndex, XINPUT_STATE* pState)
{
	return ERROR_DEVICE_NOT_CONNECTED;
}
DWORD WINAPI xinput_set_state_stub(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration)
{
	return ERROR_DEVICE_NOT_CONNECTED;
}

static xinput_get_state* XInputGetState_ = xinput_get_state_stub;
static xinput_set_state* XInputSetState_ = xinput_set_state_stub;
#define XInputGetState XInputGetState_
#define XInputSetState XInputSetState_


static void LoadXInput()
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

static void getGamepadInput(GamepadInputMap* gamepad_input)
{
	// get gamepad input
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	{
		XINPUT_STATE controller_state;
		//ZeroMemory(&state, sizeof(XINPUT_STATE));

		// Get the state of the controller from XInput
		if (XInputGetState(i, &controller_state) == ERROR_SUCCESS)	// Controller is connected
		{
			gamepad_input[i] = {};

			gamepad_input[i].buttons.up = (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
			gamepad_input[i].buttons.down = (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
			gamepad_input[i].buttons.left = (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
			gamepad_input[i].buttons.right = (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
			gamepad_input[i].buttons.start = (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_START);
			gamepad_input[i].buttons.back = (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK);
			gamepad_input[i].buttons.left_thumb = (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB);
			gamepad_input[i].buttons.right_thumb = (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB);
			gamepad_input[i].buttons.left_shoulder = (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
			gamepad_input[i].buttons.right_shoulder = (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
			gamepad_input[i].buttons.a_button = (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_A);
			gamepad_input[i].buttons.b_button = (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_B);
			gamepad_input[i].buttons.x_button = (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_X);
			gamepad_input[i].buttons.y_button = (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_Y);
						 	
			gamepad_input[i].left_trigger = controller_state.Gamepad.bLeftTrigger;
			gamepad_input[i].right_trigger = controller_state.Gamepad.bRightTrigger;
						 	
			gamepad_input[i].thumb_left_x = controller_state.Gamepad.sThumbLX;
			gamepad_input[i].thumb_left_y = controller_state.Gamepad.sThumbLY;
			gamepad_input[i].thumb_right_x = controller_state.Gamepad.sThumbRX;
			gamepad_input[i].thumb_right_y = controller_state.Gamepad.sThumbRY;
		}
		else	// Controller is not connected 
		{
			
		}
	}
}