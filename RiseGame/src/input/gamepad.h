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



void LoadXInput();

void getGamepadInput(GamepadInputMap* gamepad_input);

// TODO : find out if the user count depends on the usb hub number