#pragma once
#include "Mouse.h"
#include <SDL.h>

class ScrollBar {
	// Based on the sdl rect rapper cords

	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;

	// Fill is how far up the marker should be on the scroll bar

	int fill = 0;

public:

	int size = 0;

	// Function prototypes

	void set_cords(SDL_Rect& rect);

	// Diffrence between set fill and UpdateFill is that
	// update fill does the calculations for you based on the mouse,
	// where set fill just lets you set it manualy, but you have to do
	// the calculations.

	void UpdateFill(Mouse& mouse, bool invert = false, bool exclude_x = false);
	void setFill(int s_fill);

	int getFill();
};