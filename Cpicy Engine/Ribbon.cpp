#include "Ribbon.h"
#include "SDL+.h"
#include "Kiran.h"

Ribbon::Ribbon(std::string name, SDL_Renderer* renderer) {
	label = name;
}
void Ribbon::draw(SDL_Renderer* r, int offset) {
	if (active) {
		SDL_Rect outline{ offset * GetDesktopResolution('x') / 20 - 2, GetDesktopResolution('y') * 0.02 - 2, GetDesktopResolution('x') / 20 + 4, GetDesktopResolution('y') * 0.02 + 2 };
		SDL_SetRenderDrawColor(r, 110, 110, 110, 255);
		SDL_RenderFillRect(r, &outline);
		SDL_Rect left{ 0, GetDesktopResolution('y') * 0.04 - 2, offset * GetDesktopResolution('x') / 20, 2 };
		SDL_RenderFillRect(r, &left);
		SDL_Rect right{ offset * GetDesktopResolution('x') / 20 + GetDesktopResolution('x') / 20, GetDesktopResolution('y') * 0.04 - 2, 2000, 2};
		SDL_RenderFillRect(r, &right);
	}
	SDL_Rect background{ offset * GetDesktopResolution('x') / 20, GetDesktopResolution('y') * 0.02, GetDesktopResolution('x') / 20, GetDesktopResolution('y') * 0.021 };
	if (!active) {
		SDL_SetRenderDrawColor(r, highlight_color.r,  highlight_color.g, highlight_color.b, 255);
	}
	else {
		SDL_SetRenderDrawColor(r, background_color.r, background_color.g, background_color.b, 255);
	}
	SDL_RenderFillRect(r, &background);
	int tttw;
	int ttth;
	SDL_QueryTexture(t, NULL, NULL, &tttw, &ttth);
	SDL_Rect text_rect{ (offset * GetDesktopResolution('x') / 20) + (GetDesktopResolution('x') / 40) - (tttw/2) , GetDesktopResolution('y') * 0.02, tttw, ttth };
	SDL_RenderCopy(r, t, NULL, &text_rect);
}

bool Ribbon::update(Mouse mouse, int offset)
{
	SDL_Rect background{ offset * GetDesktopResolution('x') / 20, GetDesktopResolution('y') * 0.02, GetDesktopResolution('x') / 20, GetDesktopResolution('y') * 0.02 };
	if (IsMouseInRect(mouse, background)) {
		active = true;
		return true;
	}
	return false;
}

void Ribbon::activate(SDL_Renderer* r) {
	t = GPU_LoadText(label, r, floor(GetDesktopResolution('y') / 60), GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
}
