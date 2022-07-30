#pragma once
#include "Mouse.h"

Mouse::Mouse() {
	x = 0;
	y = 0;
	button1 = false;
	button2 = false;
	button3 = false;
	side_button1 = false;
	side_button2 = false;
}
void Mouse::UpdateMouse() {
	dx = x;
	dy = y;

	SDL_GetMouseState(&x, &y);
	dx = x - dx;
	dy = y - dy;
}
void Mouse::poll_button_presses(SDL_Event e, bool state) {
	if (state == true) {
		if (e.button.button == SDL_BUTTON_LEFT) {
			button1 = true;
		}
		else if (e.button.button == SDL_BUTTON_MIDDLE) {
			button2 = true;
		}
		else if (e.button.button == SDL_BUTTON_RIGHT) {
			button3 = true;
		}
	}
	else {
		if (e.button.button == SDL_BUTTON_LEFT) {
			button1 = false;
		}
		else if (e.button.button == SDL_BUTTON_MIDDLE) {
			button2 = false;
		}
		else if (e.button.button == SDL_BUTTON_RIGHT) {
			button3 = false;
		}
	}
}
void Mouse::get_cords(int cords[]) {
	cords[0] = x;
	cords[1] = y;
}