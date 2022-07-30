#pragma once
#include <string>
#include <SDL.h>
struct LoadedTexture {
	std::string name;
	SDL_Texture* content;
};
struct Texture {
	std::string name;
	SDL_Texture** content;
};
