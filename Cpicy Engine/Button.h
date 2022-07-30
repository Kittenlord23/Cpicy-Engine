/*
* I really do not know why this class was not created earlier during development, but all the buttons that don't follow
* this classes implmentation are a legacy way, and don't do it like that anymore. 
* Date created: June 2, 2021
*/


#pragma once
#include <SDL.h>
#include <iostream>
#include "SDL+.h"
#include "Kiran.h"

class Application;

enum class UpOrDown {
	Down = 0,
	Up = 1,
};

struct Button
{
	SDL_Texture* text;
	int width;
	int height;
	int x;
	int y;
	SDL_Color default_color;
	SDL_Color highlight_color;
	Button(std::string text, int x, int y, int w, int h, int font_size, SDL_Color color, SDL_Color hcolor, SDL_Renderer* renderer, SDL_Color text_color = SDL_Color{ 255, 255, 255 });
	void UpdateButton(Mouse* mouse, UpOrDown press_state, SDL_Event e, int id, Application* app_to_modify);
	void Draw(Mouse* mouse, SDL_Renderer* renderer);
	void InitializeButton(SDL_Renderer* renderer);
private:
	SDL_Rect getRectangle();
	void DoStuff(int id, Application* app_to_modify);
	std::string save_text;
	SDL_Color save_text_color;
	int save_font_size;
};
