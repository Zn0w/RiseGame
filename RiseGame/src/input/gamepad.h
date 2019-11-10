#pragma once

#include <windows.h>
#include <stdint.h>
#include <xinput.h>


struct GamepadInputMap {
	struct {
		uint8_t up : 1;	// the size of this member is 1 bit
		uint8_t down : 1;
		uint8_t left : 1;
		uint8_t right : 1;
		uint8_t start : 1;
		uint8_t back : 1;
		uint8_t left_thumb : 1;
		uint8_t right_thumb : 1;
		uint8_t left_shoulder : 1;
		uint8_t right_shoulder : 1;
		uint8_t a_button : 1;
		uint8_t b_button : 1;
		uint8_t x_button : 1;
		uint8_t y_button : 1;
	} buttons;

	uint8_t left_trigger;
	uint8_t right_trigger;

	uint16_t thumb_left_x;
	uint16_t thumb_left_y;
	uint16_t thumb_right_x;
	uint16_t thumb_right_y;
};


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

void LoadXInput();
//-----------------------------------------------------------------------------------------------------------------

void getGamepadInput(GamepadInputMap* gamepad_input);

// TODO : find out if the user count depends on the usb hub number