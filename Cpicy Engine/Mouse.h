#pragma once
#include <SDL.h>

struct Mouse {
	int x = 0;
	int y = 0;
	int dx = 0;
	int dy = 0;
	bool button1;
	bool button2;
	bool button3;
	bool side_button1;
	bool side_button2;

	Mouse();
	void UpdateMouse();
	void poll_button_presses(SDL_Event e, bool state);
	void get_cords(int cords[]);
};
