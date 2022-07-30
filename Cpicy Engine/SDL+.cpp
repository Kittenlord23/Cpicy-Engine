#include "SDL+.h"

bool IsMouseInRect(Mouse& mouse, SDL_Rect& rect) {
	if (mouse.x > rect.x && mouse.x < rect.x + rect.w) {
		if (mouse.y > rect.y && mouse.y < rect.y + rect.h) {
			return true;
		}
	}
	return false;
}
SDL_Surface* CPU_LoadSurface(std::string path, SDL_Surface* AppWindowSurface) {
	SDL_Surface* optimizedSurface = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		std::cout << "Error: Failed to open : " << path.c_str() << " : " << IMG_GetError() << std::endl;
	}
	else
	{
		optimizedSurface = SDL_ConvertSurface(loadedSurface, AppWindowSurface->format, 0);
		if (optimizedSurface == NULL)
		{
			std::cout << "Failed to optomize image : " << SDL_GetError() << std::endl;
		}

		SDL_FreeSurface(loadedSurface);
	}

	return optimizedSurface;
}
SDL_Texture* GPU_LoadTexture(std::string path, SDL_Renderer* AppRenderer) {
	SDL_Texture* finalTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		std::cout << "Error : Failed to open : " << path.c_str() << ": " << IMG_GetError() << std::endl;
	}
	else
	{
		finalTexture = SDL_CreateTextureFromSurface(AppRenderer, loadedSurface);
		if (finalTexture == NULL)
		{
			std::cout << "Error : Unable to create texture from surface : " << SDL_GetError() << std::endl;
		}
		SDL_FreeSurface(loadedSurface);
	}
	return finalTexture;
}
SDL_Texture* GPU_LoadText(std::string text, SDL_Renderer* AppRenderer, int font_size, std::string FontPath, SDL_Color color) {
	// Open font

	TTF_Font* f = TTF_OpenFont(FontPath.c_str(), font_size);

	// I use blended becuase it looks awful without it IMO
  	SDL_Surface* text_surface = TTF_RenderText_Blended(f, text.c_str(), color);
	// Return the texture version
	return SDL_CreateTextureFromSurface(AppRenderer, text_surface);
}
SDL_KeyCode GetKeyFromString(std::string character) {
	if (character == "a")
		return SDLK_a;
	if (character == "b")
		return SDLK_b;
	if (character == "c")
		return SDLK_c;
	if (character == "d")
		return SDLK_d;
	if (character == "e")
		return SDLK_e;
	if (character == "f")
		return SDLK_f;
	if (character == "g")
		return SDLK_g;
	if (character == "h")
		return SDLK_h;
	if (character == "i")
		return SDLK_i;
	if (character == "j")
		return SDLK_j;
	if (character == "k")
		return SDLK_k;
	if (character == "l")
		return SDLK_l;
	if (character == "m")
		return SDLK_m;
	if (character == "n")
		return SDLK_n;
	if (character == "o")
		return SDLK_o;
	if (character == "p")
		return SDLK_p;
	if (character == "q")
		return SDLK_q;
	if (character == "r")
		return SDLK_r;
	if (character == "s")
		return SDLK_s;
	if (character == "t")
		return SDLK_t;
	if (character == "u")
		return SDLK_u;
	if (character == "v")
		return SDLK_v;
	if (character == "w")
		return SDLK_w;
	if (character == "x")
		return SDLK_x;
	if (character == "y")
		return SDLK_y;
	if (character == "z")
		return SDLK_z;
	if (character == "1")
		return SDLK_1;
	if (character == "2")
		return SDLK_2;
	if (character == "3")
		return SDLK_3;
	if (character == "4")
		return SDLK_4;
	if (character == "5")
		return SDLK_5;
	if (character == "6")
		return SDLK_6;
	if (character == "7")
		return SDLK_7;
	if (character == "8")
		return SDLK_8;
	if (character == "9")
		return SDLK_9;
	if (character == "0")
		return SDLK_0;
	if (character == ";")
		return SDLK_SEMICOLON;
	if (character == ",")
		return SDLK_COMMA;
	if (character == ":")
		return SDLK_COLON;
	if (character == "[")
		return SDLK_LEFTBRACKET;
	if (character == "]")
		return SDLK_RIGHTBRACKET;
	else
		throw 5;
}
std::string GetStringFromKey(SDL_Keycode character) {
	if (character == SDLK_a)
		return std::string("a");
	if (character == SDLK_b)
		return std::string("b");
	if (character == SDLK_c)
		return std::string("c");
	if (character == SDLK_d)
		return std::string("d");
	if (character == SDLK_e)
		return std::string("e");
	if (character == SDLK_f)
		return std::string("f");
	if (character == SDLK_g)
		return std::string("g");
	if (character == SDLK_h)
		return std::string("h");
	if (character == SDLK_i)
		return std::string("i");
	if (character == SDLK_j)
		return std::string("j");
	if (character == SDLK_k)
		return std::string("k");
	if (character == SDLK_l)
		return std::string("l");
	if (character == SDLK_m)
		return std::string("m");
	if (character == SDLK_n)
		return std::string("n");
	if (character == SDLK_o)
		return std::string("o");
	if (character == SDLK_p)
		return std::string("p");
	if (character == SDLK_q)
		return std::string("q");
	if (character == SDLK_r)
		return std::string("r");
	if (character == SDLK_s)
		return std::string("s");
	if (character == SDLK_t)
		return std::string("t");
	if (character == SDLK_u)
		return std::string("u");
	if (character == SDLK_v)
		return std::string("v");
	if (character == SDLK_w)
		return std::string("w");
	if (character == SDLK_x)
		return std::string("x");
	if (character == SDLK_y)
		return std::string("y");
	if (character == SDLK_z)
		return std::string("z");
	if (character == SDLK_1)
		return std::string("1");
	if (character == SDLK_2)
		return std::string("2");
	if (character == SDLK_3)
		return std::string("3");
	if (character == SDLK_4)
		return std::string("4");
	if (character == SDLK_5)
		return std::string("5");
	if (character == SDLK_6)
		return std::string("6");
	if (character == SDLK_7)
		return std::string("7");
	if (character == SDLK_8)
		return std::string("8");
	if (character == SDLK_9)
		return std::string("9");
	if (character == SDLK_0)
		return std::string("0");
}