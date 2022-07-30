#include "Button.h"
#include "App.h"

Button::Button(std::string string, int x, int y, int w, int h, int font_size, SDL_Color color, SDL_Color hcolor, SDL_Renderer* renderer, SDL_Color text_color) {
	save_text = string;
	save_text_color = text_color;
	save_font_size = font_size;
	width = w;
	height = h;
	x = x;
	y = y;
	highlight_color = hcolor;
	default_color = color;
}
void Button::UpdateButton(Mouse* mouse, UpOrDown press_state, SDL_Event e, int id, Application* app_to_modify) {
	auto r = getRectangle();
	if (IsMouseInRect(*mouse, r)) {
		if (press_state == UpOrDown::Down) {
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				DoStuff(id, app_to_modify);
			}
		}
		else {
			if (e.type == SDL_MOUSEBUTTONUP) {
				DoStuff(id, app_to_modify);
			}
		}
	}
}
void Button::Draw(Mouse* mouse, SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, default_color.r, default_color.g, default_color.b, 255);
	SDL_Rect r{ x, y, width, height };
	if (IsMouseInRect(*mouse, r)) {
		SDL_SetRenderDrawColor(renderer, highlight_color.r, highlight_color.g, highlight_color.b, 255);
	}
	SDL_RenderFillRect(renderer, &r);
	int text_width;
	int text_height;
	SDL_QueryTexture(text, NULL, NULL, &text_width, &text_height);
	SDL_Rect text_rect{ x - text_width / 4 + width / 3, y - text_height / 4 + height / 2, text_width / 2, text_height / 2};
	SDL_RenderCopy(renderer, text, NULL, &text_rect);
}
void Button::InitializeButton(SDL_Renderer* renderer)
{
	text = GPU_LoadText(save_text, renderer, save_font_size, GetExecutablePath() + "Extern\\Fonts\\arial.ttf", save_text_color);
}
SDL_Rect Button::getRectangle() {
	SDL_Rect t{ x, y, width, height };
	return t;
}
void Button::DoStuff(int id, Application* app_to_modify) {

}