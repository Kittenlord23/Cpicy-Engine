#pragma once
#include "ScrollBar.h"

void ScrollBar::set_cords(SDL_Rect& rect) {
	x = rect.x;
	y = rect.y;
	w = rect.w;
	h = rect.h;
}

void ScrollBar::UpdateFill(Mouse& mouse, bool invert, bool exclude_x) {
	if (mouse.x > x && mouse.x < x + w) {
		if (mouse.y > y && mouse.y + h) {
			if (!invert) {
				fill = mouse.y - y;
			}
			else {
				fill = y + h - mouse.y - size;
			}
		}
	}
}
int ScrollBar::getFill() {
	return fill;
}
void ScrollBar::setFill(int s_fill) {
	fill = s_fill;
}