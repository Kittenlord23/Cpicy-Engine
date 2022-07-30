#pragma once
#include <string>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Mouse.h"

// Used to compare mouse cords to rectangle cords
// Returns true if the mouse persented is inside the rectangle
// and returns false if it not.

bool IsMouseInRect(Mouse&, SDL_Rect& rect);

// Used to loadings for surfaces, or "cpu" images
// Takes a path, and a screen surface, and returns
// a SDL surface pointer for blitting

SDL_Surface* CPU_LoadSurface(std::string path, SDL_Surface* AppWindowSurface);

// Used to load textures or "gpu" images
// Takes a path, and a renderer, and returns
// Texture pointer of the texture for rendering

SDL_Texture* GPU_LoadTexture(std::string path, SDL_Renderer* AppRenderer);

// Used to generate textures used for text boxes
// Often the generated text the fontsize is redundent, but bigger
// fonts will make it more clear but also
// take more memory(Check spelling)

SDL_Texture* GPU_LoadText(std::string text, SDL_Renderer* AppRenderer, int font_size, std::string FontPath, SDL_Color color = { 255, 255, 255 });

SDL_KeyCode GetKeyFromString(std::string character);
std::string GetStringFromKey(SDL_Keycode character);