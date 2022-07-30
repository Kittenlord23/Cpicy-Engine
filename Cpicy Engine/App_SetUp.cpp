#include "App.h"
#include <SDL_mixer.h>

// Used to get the quite state inside the main loop


bool Application::GetQuitState() {
	return should_quit;
}

bool Application::initialize() {
	// Initialize Sdl

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		std::cout << "Error : SDL_Init failed: " << SDL_GetError() << std::endl;
		return false;
	}
	else {
		// Report an error if texture filtering failed

		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
			std::cout << "Warning : Texture Filtering failed" << std::endl;

		// Create the window

		AppWindow = SDL_CreateWindow("Cpicy Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 852, 480, SDL_WINDOW_SHOWN);

		if (AppWindow == NULL)
		{
			std::cout << "Error : Window could not be created" << SDL_GetError() << std::endl;
			return false;
		}
		else {
			// Set it to bordered and make a CPU surface
			// and set the window cpu context to that surface

			SDL_SetWindowBordered(AppWindow, SDL_FALSE);
			AppWindowSurface = SDL_GetWindowSurface(AppWindow);

			// Create the renderer

			AppRenderer = SDL_CreateRenderer(AppWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (AppRenderer == NULL)
			{
				// Create warnings about it if something goes wrong

				std::cout << "Error : Renderer could not be created: " << SDL_GetError() << std::endl;
				return false;
			}
			else
			{
				// Initialize other sdl libraries

				SDL_SetRenderDrawColor(AppRenderer, 255, 255, 255, 255);

				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					std::cout << "Error : SDL_image could not initialize:" << IMG_GetError() << std::endl;
					return false;
				}
				if (TTF_Init() < 0)
				{
					std::cout << "Error : SDL_ttf could not initialize:" << TTF_GetError() << std::endl;
					return false;
				}
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					std::cout << "Error : SDL_mixer could not initialize: " <<   Mix_GetError();
					return false;
				}
				SDL_UpdateWindowSurface(AppWindow);
				editor.current_texture_index = 0;
				GeneralSectionRibbon.activate(AppRenderer);
				GraphicSectionRibbon.activate(AppRenderer);
				InteractiveSectionRibbon.activate(AppRenderer);
				LayerSectionRibbon.activate(AppRenderer);
				EditSectionRibbon.activate(AppRenderer);
				ribbons.push_back(&GeneralSectionRibbon);
				ribbons.push_back(&GraphicSectionRibbon);
				ribbons.push_back(&InteractiveSectionRibbon);
				ribbons.push_back(&LayerSectionRibbon);
				ribbons.push_back(&EditSectionRibbon);
			}
		}
	}

	// Returns true if it everything was initilized and and did not
	// return early with false

	return true;
}
void Application::close()
{
	SDL_FreeSurface(Startup480A_Surface);
	SDL_DestroyRenderer(AppRenderer);
	SDL_DestroyWindow(AppWindow);
	AppWindow = NULL;
	AppRenderer = NULL;
	IMG_Quit();
	TTF_Quit();
	Mix_Quit();
	SDL_Quit();
}