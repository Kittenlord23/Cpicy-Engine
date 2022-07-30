#include "InputBox.h"
#include "SDL+.h"
#include "Kiran.h"

InputBox::InputBox(int width, int height, std::string title, std::string poll, int max_reponse_length, bool allow_cancel, bool number_only, bool allow_data_macro, std::string cancel_response) {
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_SHOWN, &window, &renderer);
	SDL_Surface* Icon = CPU_LoadSurface(GetExecutablePath() + "Extern\\Assets\\Images\\CPU\\Images\\InputIcon.png", SDL_GetWindowSurface(window));
	SDL_SetWindowIcon(window, Icon);
	SDL_SetWindowTitle(window, title.c_str());
	SDL_Event event;
	std::string local_return = cancel_response;
	bool quit = false;
	SDL_Color black = { 0, 0, 0 };
	SDL_Texture* PollText = GPU_LoadText(poll + ": " + local_return, renderer, 15,  GetExecutablePath() + "Extern\\Fonts\\arial.ttf", black);
	SDL_Texture* ConfirmButton = GPU_LoadText("Ok", renderer, 15, GetExecutablePath() + "Extern\\Fonts\\arial.ttf", black);
	SDL_Texture* DataMacro = GPU_LoadText("Get value", renderer, 15, GetExecutablePath() + "Extern\\Fonts\\arial.ttf", black);
	Mouse _mouse{};
	while (!quit) {
		_mouse.UpdateMouse();
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);
		int plttw;
		int pltth;
		SDL_QueryTexture(PollText, NULL, NULL, &plttw, &pltth);
		SDL_Rect PollTextRect{ width/8, height/3, plttw, pltth };
		SDL_RenderCopy(renderer, PollText, NULL, &PollTextRect);
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE) {
				if (SDL_GetWindowID(window) == event.window.windowID) {
					quit = true;
					if (allow_cancel) {
						local_return = cancel_response;
					}
					else {
						this->return_value = local_return;
					}
				}
			}
			if (event.type == SDL_TEXTINPUT && !number_only) {
				if (!(SDL_GetModState() & KMOD_CTRL && (event.text.text[0] == 'c' || event.text.text[0] == 'C' || event.text.text[0] == 'v' || event.text.text[0] == 'V')))
				{
					if (local_return.size() < max_reponse_length)
					local_return += event.text.text;
					PollText = GPU_LoadText(poll + ": " + local_return, renderer, 15, GetExecutablePath() + "Extern\\Fonts\\arial.ttf", black);
				}
			}
			if (number_only) {
				if (event.type == SDL_KEYDOWN) {
					if (event.key.keysym.sym == SDLK_0) {
						if (local_return.size() < max_reponse_length) {
							local_return += "0";
							PollText = GPU_LoadText(poll + ": " + local_return, renderer, 15, GetExecutablePath() + "Extern\\Fonts\\arial.ttf", black);
						}
					}
					if (event.key.keysym.sym == SDLK_1) {
						if (local_return.size() < max_reponse_length) {
							local_return += "1";
							PollText = GPU_LoadText(poll + ": " + local_return, renderer, 15, GetExecutablePath() + "Extern\\Fonts\\arial.ttf", black);
						}
					}
					if (event.key.keysym.sym == SDLK_2) {
						if (local_return.size() < max_reponse_length) {
							local_return += "2";
							PollText = GPU_LoadText(poll + ": " + local_return, renderer, 15, GetExecutablePath() + "Extern\\Fonts\\arial.ttf", black);
						}
					}
					if (event.key.keysym.sym == SDLK_3) {
						if (local_return.size() < max_reponse_length) {
							local_return += "3";
							PollText = GPU_LoadText(poll + ": " + local_return, renderer, 15, GetExecutablePath() + "Extern\\Fonts\\arial.ttf", black);
						}
					}
					if (event.key.keysym.sym == SDLK_4) {
						if (local_return.size() < max_reponse_length) {
							local_return += "4";
							PollText = GPU_LoadText(poll + ": " + local_return, renderer, 15, GetExecutablePath() + "Extern\\Fonts\\arial.ttf", black);
						}
					}
					if (event.key.keysym.sym == SDLK_5) {
						if (local_return.size() < max_reponse_length) {
							local_return += "5";
							PollText = GPU_LoadText(poll + ": " + local_return, renderer, 15, GetExecutablePath() + "Extern\\Fonts\\arial.ttf", black);
						}
					}
					if (event.key.keysym.sym == SDLK_6) {
						if (local_return.size() < max_reponse_length) {
							local_return += "6";
							PollText = GPU_LoadText(poll + ": " + local_return, renderer, 15, GetExecutablePath() + "Extern\\Fonts\\arial.ttf", black);
						}
					}
					if (event.key.keysym.sym == SDLK_7) {
						if (local_return.size() < max_reponse_length) {
							local_return += "7";
							PollText = GPU_LoadText(poll + ": " + local_return, renderer, 15, GetExecutablePath() + "Extern\\Fonts\\arial.ttf", black);
						}
					}
					if (event.key.keysym.sym == SDLK_8) {
						if (local_return.size() < max_reponse_length) {
							local_return += "8";
							PollText = GPU_LoadText(poll + ": " + local_return, renderer, 15, GetExecutablePath() + "Extern\\Fonts\\arial.ttf", black);
						}
					}
					if (event.key.keysym.sym == SDLK_9) {
						if (local_return.size() < max_reponse_length) {
							local_return += "9";
							PollText = GPU_LoadText(poll + ": " + local_return, renderer, 15, GetExecutablePath() + "Extern\\Fonts\\arial.ttf", black);
						}
					}
					if (event.key.keysym.sym == SDLK_PERIOD) {
						if (local_return.size() < max_reponse_length) {
							if (local_return.find(".") == std::string::npos) {
								local_return += ".";
								PollText = GPU_LoadText(poll + ": " + local_return, renderer, 15, GetExecutablePath() + "Extern\\Fonts\\arial.ttf", black);
							}
						}
					}
				}
			}
			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_BACKSPACE) {
					if (local_return.size() != 0) {
						local_return.pop_back();
						PollText = GPU_LoadText(poll + ": " + local_return, renderer, 15, GetExecutablePath() + "Extern\\Fonts\\arial.ttf", black);
					}
				}
			}
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				int cfttw;
				int cftth;
				SDL_QueryTexture(ConfirmButton, NULL, NULL, &cfttw, &cftth);
				SDL_Rect ConfirmButtonRect{ width * 0.85 - cfttw, height * 0.85 - cftth, width * 0.1 + cfttw * 2, height * 0.1 + cftth * 2 };
				if (IsMouseInRect(_mouse, ConfirmButtonRect)) {
					this->return_value = local_return;
 					quit = true;
				}
				if (allow_data_macro) {
					SDL_Rect DataMacroButtonRect{ width * 0.85 - cfttw * 4.2, height * 0.85 - cftth, width * 0.1 + cfttw * 2, height * 0.1 + cftth * 2 };
					if (IsMouseInRect(_mouse, DataMacroButtonRect)) {
						local_return += "%data%";
						PollText = GPU_LoadText(poll + ": " + local_return, renderer, 15, GetExecutablePath() + "Extern\\Fonts\\arial.ttf", black);
					}
				}
			}
		}
		
		int cfttw;
		int cftth;
		SDL_QueryTexture(ConfirmButton, NULL, NULL, &cfttw, &cftth);
		SDL_Rect ConfirmButtonRect{ width * 0.85 - cfttw, height * 0.85 - cftth, width * 0.1 + cfttw * 2, height * 0.1 + cftth * 2 };
		SDL_Rect ConfirmButtonTextRect{ width * 0.85, height * 0.85, cfttw, cftth};
		SDL_SetRenderDrawColor(renderer, 50, 150, 50, 255);
		if (IsMouseInRect(_mouse, ConfirmButtonRect)) {
			SDL_SetRenderDrawColor(renderer, 50, 255, 50, 255);
		}
		SDL_RenderFillRect(renderer, &ConfirmButtonRect);
		SDL_RenderCopy(renderer, ConfirmButton, NULL, &ConfirmButtonTextRect);
		if (allow_data_macro) {
			SDL_Rect DataMacroButtonRect{ width * 0.85 - cfttw * 4.2, height * 0.85 - cftth, width * 0.1 + cfttw * 2, height * 0.1 + cftth * 2 };
			SDL_SetRenderDrawColor(renderer, 50, 150, 50, 255);
			if (IsMouseInRect(_mouse, DataMacroButtonRect)) {
				SDL_SetRenderDrawColor(renderer, 50, 255, 50, 255);
			}
			SDL_RenderFillRect(renderer, &DataMacroButtonRect);
			int dmttw;
			int dmtth;
			SDL_QueryTexture(DataMacro, NULL, NULL, &dmttw, &dmtth);
			SDL_Rect DataMacroTextRect { width * 0.83 - cfttw * 4, height * 0.9 -  cftth, dmttw, dmtth };
			SDL_RenderCopy(renderer, DataMacro, NULL, &DataMacroTextRect);
			
		}
		SDL_RenderPresent(renderer);
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

std::string InputBox::GetInputFromBox() {
	return this->return_value;
}