#pragma once
#include <iostream>
#include <SDL.h>
#include <vector>
#include "Mouse.h"

class Ribbon {
	SDL_Texture* t;
	SDL_Color background_color = { 45, 45, 48 };
	SDL_Color highlight_color = { 90, 90, 90 };
	bool first_time_load = false;
	std::string label = "";
public:
	bool active;
public:
	Ribbon(std::string name, SDL_Renderer* renderer);
	void draw(SDL_Renderer* r, int offset);
	bool update(Mouse mouse, int offset);
	void activate(SDL_Renderer* r);
};