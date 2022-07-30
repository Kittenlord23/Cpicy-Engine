#include "App.h"
#include <SDL.h>
#include "SDL+.h"
#include <string>

void Application::AboutPage() {
	SDL_Window* window;
	SDL_Renderer* renderer;
	int width = GetDesktopResolution('x') / 4;
	int height = GetDesktopResolution('y') / 2;
	short state = 0;
	window = SDL_CreateWindow("Help", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Event e;
	bool quit = false;
	Mouse mouse;
	SDL_Texture* HelpText = GPU_LoadText("Help", renderer, GetDesktopResolution('x') / 60, GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	SDL_Texture* AboutText = GPU_LoadText("About", renderer, GetDesktopResolution('x') / 60, GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	SDL_Texture* LicenseText = GPU_LoadText("License(SDL2)", renderer, GetDesktopResolution('x') / 60, GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	SDL_Texture* LicenseImage = GPU_LoadTexture(GetExecutablePath() + "Extern\\Assets\\Images\\GPU\\Text\\SDL2 License.png", renderer);
	SDL_Texture* AuthorText = GPU_LoadText("Made by Kiran Sagbohan", renderer, GetDesktopResolution('x') / 77, GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	SDL_Texture* AuthorEmail = GPU_LoadText("<k0sagbohan@gmail.com>", renderer, GetDesktopResolution('x') / 77, GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	SDL_Texture* SplashArt = GPU_LoadText("Splash Art by Abiola Sagbohan", renderer, GetDesktopResolution('x') / 77, GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	SDL_Texture* SplashArtEmail = GPU_LoadText("<EmailGoesHere@something.huh>", renderer, GetDesktopResolution('x') / 77, GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	while (!quit) {
		SDL_SetRenderDrawColor(renderer, 45, 45, 48, 255);
		SDL_RenderClear(renderer);
		mouse.UpdateMouse();
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE) {
				if (SDL_GetWindowID(window) == e.window.windowID)
					quit = true;
			}
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				int ltttw;
				int lttth;
				SDL_QueryTexture(LicenseText, NULL, NULL, &ltttw, &lttth);
				SDL_Rect LicenseTextRect{ width * 0.5, height * 0.05, ltttw, lttth };
				if (IsMouseInRect(mouse, LicenseTextRect)) {
					state = 2;
				}
				int atttw;
				int attth;
				SDL_QueryTexture(AboutText, NULL, NULL, &atttw, &attth);
				SDL_Rect AboutTextRect{ width * 0.05, height * 0.05, atttw, attth };
				if (IsMouseInRect(mouse, AboutTextRect)) {
					state = 0;
				}

			}
			
		}
		int atttw;
		int attth;
		SDL_QueryTexture(AboutText, NULL, NULL, &atttw, &attth);
		SDL_Rect AboutTextRect{ width * 0.05, height * 0.05, atttw, attth };
		SDL_RenderCopy(renderer, AboutText, NULL, &AboutTextRect);
		int htttw;
		int httth;
		SDL_QueryTexture(HelpText, NULL, NULL, &htttw, &httth);
		SDL_Rect HelpTextRect{ width * 0.37 - htttw/2, height * 0.05, htttw, httth };
		//SDL_RenderCopy(renderer, HelpText, NULL, &HelpTextRect);
		int ltttw;
		int lttth;
		SDL_QueryTexture(LicenseText, NULL, NULL, &ltttw, &lttth);
		SDL_Rect LicenseTextRect{ width * 0.5, height * 0.05, ltttw, lttth };
		SDL_RenderCopy(renderer, LicenseText, NULL, &LicenseTextRect);

		if (state == 2) {
			int littw;
			int litth;
			int w;
			SDL_GetWindowSize(window, &w, NULL);
			SDL_QueryTexture(LicenseImage, NULL, NULL, &littw, &litth);
			if (w != littw) {
				SDL_SetWindowSize(window, littw, litth + window_height * 0.06);
			}
			SDL_Rect LicenseImageRect{ width * 0, height * 0.12, littw, litth };
			SDL_RenderCopy(renderer, LicenseImage, NULL, &LicenseImageRect);
		}
		else {
			int w;
			SDL_GetWindowSize(window, &w, NULL);
			if (w != width) {
				SDL_SetWindowSize(window, width, height);
			}
		}

		if (state == 0) {
			int attw;
			int atth;
			SDL_QueryTexture(AuthorText, NULL, NULL, &attw, &atth);
			SDL_Rect AuthorTextRect{window_width * 0.01, window_height * 0.1, attw, atth};
			SDL_RenderCopy(renderer, AuthorText, NULL, &AuthorTextRect);
			SDL_QueryTexture(SplashArt, NULL, NULL, &attw, &atth);
			SDL_Rect SplashTextRect{ window_width * 0.01, window_height * 0.3, attw, atth };
			SDL_RenderCopy(renderer, SplashArt, NULL, &SplashTextRect);
			SDL_QueryTexture(AuthorEmail, NULL, NULL, &attw, &atth);
			SDL_Rect AuthorTextEmailRect{ window_width * 0.01, window_height * 0.15, attw, atth };
			SDL_RenderCopy(renderer, AuthorEmail, NULL, &AuthorTextEmailRect);
			SDL_QueryTexture(SplashArtEmail, NULL, NULL, &attw, &atth);
			SDL_Rect SplashTextEmailRect{ window_width * 0.01, window_height * 0.35, attw, atth };
			SDL_RenderCopy(renderer, SplashArtEmail, NULL, &SplashTextEmailRect);
		}

		SDL_RenderPresent(renderer);
		SDL_Delay(1000 / 30);
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}