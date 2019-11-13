#pragma once


void game_init();
void game_update();
void game_destroy();

// returns a user-defined resolution
void get_user_resolution(int* screen_width, int* screen_height);