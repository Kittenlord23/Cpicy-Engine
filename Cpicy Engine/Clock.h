#pragma once
#include <SDL.h>
struct Clock
{
	Uint32 lastupdate = SDL_GetTicks();
	Uint32 current = 0;
	double deltaTime = 0;
};