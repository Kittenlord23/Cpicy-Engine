#include "App.h"
#pragma warning(push)
#pragma warning(disable : 4838)
#pragma warning(disable: 4244)
#pragma warning(disable: 4267)

void Application::ColorPicker() {
	bool exit_color_picker = false;
	SDL_Window* color_picker_window = SDL_CreateWindow("Choose color", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, SDL_WINDOW_SHOWN);
	SDL_Renderer* color_picker_renderer = SDL_CreateRenderer(color_picker_window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Surface* color_picker_surface = SDL_GetWindowSurface(color_picker_window);
	SDL_Event e;
	int color[3] = { 255, 0, 0 };
	int final_color[3] = { 255, 255 , 255 };
	SDL_Surface* color_picker_icon = CPU_LoadSurface(GetExecutablePath() + "Extern\\Assets\\Images\\CPU\\Images\\ColorChooserIcon32.png", color_picker_surface);
	SDL_SetWindowIcon(color_picker_window, color_picker_icon);
	SDL_FreeSurface(color_picker_icon);
	color_picker_icon = NULL;
	// Bliue and green got switched around
	SDL_Texture* RedTextTexture = GPU_LoadText("Red: " + std::to_string(final_color[0]), color_picker_renderer, 24, GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	SDL_Texture* BlueTextTexture = GPU_LoadText("Green: " + std::to_string(final_color[1]), color_picker_renderer, 24, GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	SDL_Texture* GreenTextTexture = GPU_LoadText("Blue: " + std::to_string(final_color[2]), color_picker_renderer, 24, GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	SDL_Texture* OKButtonTextTexture = GPU_LoadText("Confirm", color_picker_renderer, 24, GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	ScrollBar color_bar{};
	Mouse color_picker_mouse = Mouse();
	bool editing_color = false;
	bool editing_hue = false;
	SDL_Rect HuePicker{ 50, 50, 500, 500 };
	SDL_Rect ColorBar{ 700, 50, 40, 600 };
	SDL_Rect FinalColorRect{ 50, 575, 500, 25 };
	SDL_Rect HueMarker{ 48, 48, 4, 4 };
	while (!exit_color_picker) {
		SDL_Rect TopBar{ 0, 0, 800, 50 };
		SDL_Rect SideBar{ 0, 50, 50, 750 };
		SDL_Rect MidRect{ 550, 50, 250, 750 };
		SDL_Rect BottemRect{ 50, 550, 500, 250 };
		SDL_SetRenderDrawColor(color_picker_renderer, 45, 45, 48, 255);
		SDL_RenderFillRect(color_picker_renderer, &TopBar);
		SDL_RenderFillRect(color_picker_renderer, &SideBar);
		SDL_RenderFillRect(color_picker_renderer, &MidRect);
		SDL_RenderFillRect(color_picker_renderer, &BottemRect);
		int rttw;
		int rtth;
		SDL_QueryTexture(RedTextTexture, NULL, NULL, &rttw, &rtth);
		SDL_Rect RedTextRect{ 50, 700, rttw, rtth };
		SDL_RenderCopy(color_picker_renderer, RedTextTexture, NULL, &RedTextRect);
		int bttw;
		int btth;
		SDL_QueryTexture(BlueTextTexture, NULL, NULL, &bttw, &btth);
		SDL_Rect BlueTextRect{ 250, 700, bttw, btth };
		SDL_RenderCopy(color_picker_renderer, BlueTextTexture, NULL, &BlueTextRect);
		int gttw;
		int gtth;
		SDL_QueryTexture(GreenTextTexture, NULL, NULL, &gttw, &gtth);
		SDL_Rect GreenTextRect{ 450, 700, gttw, gtth };
		SDL_RenderCopy(color_picker_renderer, GreenTextTexture, NULL, &GreenTextRect);
		SDL_Rect OkButtonRect{ 650, 700, 125, 60 };
		int okttw;
		int oktth;
		SDL_QueryTexture(OKButtonTextTexture, NULL, NULL, &okttw, &oktth);
		SDL_SetRenderDrawColor(color_picker_renderer, 50, 250, 50, 255);
		SDL_Rect OkButtonTextRect{ 670, 715, okttw, oktth };
		SDL_RenderFillRect(color_picker_renderer, &OkButtonRect);
		SDL_RenderCopy(color_picker_renderer, OKButtonTextTexture, NULL, &OkButtonTextRect);
		color_picker_mouse.UpdateMouse();												// Update mouse position
		while (SDL_PollEvent(&e))														// Poll events
		{
			if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE) {
				// so this handels when the color picker is closed
				if (SDL_GetWindowID(color_picker_window) == e.window.windowID)
				{
					SDL_DestroyRenderer(color_picker_renderer);
					SDL_DestroyWindow(color_picker_window);
					color_picker_renderer = NULL;
					color_picker_window = NULL;
					exit_color_picker = true;
				}
			}
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				if (e.button.button == SDL_BUTTON_LEFT) {
					if (IsMouseInRect(color_picker_mouse, ColorBar)) {
						if (editing_color) {
							editing_color = false;
						}
						else {
							editing_color = true;
						}
					}
				}
				if (IsMouseInRect(color_picker_mouse, HuePicker)) {
					editing_hue = true;
				}
				if (IsMouseInRect(color_picker_mouse, OkButtonRect)) {
					SDL_DestroyRenderer(color_picker_renderer);							// Clean up resources
					SDL_DestroyWindow(color_picker_window);
					color_picker_renderer = NULL;
					color_picker_window = NULL;
					exit_color_picker = true;
					editor.color[0] = final_color[0]; editor.color[1] = final_color[1]; editor.color[2] = final_color[2]; // Return the final color to the main application
				}
			}
			if (e.type == SDL_MOUSEBUTTONUP) {
				if (e.button.button == SDL_BUTTON_LEFT) {
					editing_hue = false;
					for (int x = 0; x <= 500; x++) {
						for (int y = 0; y <= 500; y++) {
							// Super cool code
							float white_percent = x / 500.0f;
							float black_percent = y / 500.0f;
							int pixel_color[3] = { 0, 0, 0 };
							pixel_color[0] = color[0] + ((255 - color[0]) * white_percent) - (color[0] * black_percent);
							pixel_color[1] = color[1] + ((255 - color[1]) * white_percent) - (color[1] * black_percent);
							pixel_color[2] = color[2] + ((255 - color[2]) * white_percent) - (color[2] * black_percent);
							SDL_SetRenderDrawColor(color_picker_renderer, pixel_color[0], pixel_color[1], pixel_color[2], 255);
							SDL_RenderDrawPoint(color_picker_renderer, 550 - x, y + 50);
						}
					}
				}
			}
		}
		if (editing_hue) {
			// Begin complicated math
			// the -50.0f is the offset that the rectangle has, so we subtract it
			// to get the postion of the mouse relitive to the corrnor of the rectangle
			// next divide by 500.0f because that is the width of the rectangle,
			// to get the final value as a float between 0.0f and 1.0f
			// Do the same for white and black
			float white_percent = (550 - color_picker_mouse.x) / 500.0f;
			float black_percent = (color_picker_mouse.y - 50.0f) / 500.0f;
			// Then, take the color of the color bar, and add the calculated values
			// for the hue slider along the white percet, or x axis.
			// Then subtract it by the black percent, or y axis to
			// assign the final color
			HueMarker.x = color_picker_mouse.x - 2;
			HueMarker.y = color_picker_mouse.y - 2;
			if (HueMarker.x + 2 > 550) {
				HueMarker.x = 550;
			}
			if (HueMarker.y + 2 > 550) {
				HueMarker.y = 550;
			}
			if (HueMarker.x < 48) {
				HueMarker.x = 48;
			}
			if (HueMarker.y < 48) {
				HueMarker.y = 48;
			}
			final_color[0] = color[0] + ((255 - color[0]) * white_percent) - (color[0] * black_percent);
			final_color[1] = color[1] + ((255 - color[1]) * white_percent) - (color[1] * black_percent);
			final_color[2] = color[2] + ((255 - color[2]) * white_percent) - (color[2] * black_percent);
			if (final_color[0] > 255)
				final_color[0] = 255;
			if (final_color[1] > 255)
				final_color[1] = 255;
			if (final_color[2] > 255)
				final_color[2] = 255;
			if (final_color[0] < 0)
				final_color[0] = 0;
			if (final_color[1] < 0)
				final_color[1] = 0;
			if (final_color[1] < 0)
				final_color[1] = 0;
			if (final_color[0] < 0)
				final_color[0] = 0;
			if (final_color[1] < 0)
				final_color[1] = 0;
			if (final_color[2] < 0)
				final_color[2] = 0;
			RedTextTexture = GPU_LoadText("Red: " + std::to_string(final_color[0]), color_picker_renderer, 24, GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
			BlueTextTexture = GPU_LoadText("Green: " + std::to_string(final_color[1]), color_picker_renderer, 24, GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
			GreenTextTexture = GPU_LoadText("Blue: " + std::to_string(final_color[2]), color_picker_renderer, 24, GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
		}

		// For when the user is changing the color bar

		if (!IsMouseInRect(color_picker_mouse, ColorBar))
			editing_color = false;

		SDL_SetRenderDrawColor(color_picker_renderer, 100, 100, 100, 255);
		SDL_RenderFillRect(color_picker_renderer, &ColorBar);
		color_bar.set_cords(ColorBar);
		for (int x = 0; x <= 600; x++) {
			SDL_Rect LineFragment{ 700, 50 + x, 40, 1 };
			if (x <= 100) {
				SDL_SetRenderDrawColor(color_picker_renderer, 255, int(x * 2.5), 0, 255);
			}
			else if (x > 100 && x <= 200) {
				SDL_SetRenderDrawColor(color_picker_renderer, 255 - int((x - 100) * 2.5), 255, 0, 255);
			}
			else if (x > 200 && x <= 300) {
				SDL_SetRenderDrawColor(color_picker_renderer, 0, 255, int((x - 200) * 2.5), 255);
			}
			else if (x > 300 && x <= 400) {
				SDL_SetRenderDrawColor(color_picker_renderer, 0, 255 - int((x - 300) * 2.5), 255, 255);
			}
			else if (x > 400 && x <= 500) {
				SDL_SetRenderDrawColor(color_picker_renderer, int((x - 400) * 2.5), 0, 255, 255);
			}
			else if (x > 500 && x <= 600) {
				SDL_SetRenderDrawColor(color_picker_renderer, 255, 0, 255 - int((x - 500) * 2.5), 255);
			}
			SDL_RenderFillRect(color_picker_renderer, &LineFragment);
		}
		SDL_SetRenderDrawColor(color_picker_renderer, 255, 255, 255, 255);
		
		if (editing_color)
			if (color_bar.getFill() <= 600) {
				color_bar.UpdateFill(color_picker_mouse);
				if (color_bar.getFill() <= 100) {
					color[0] = 255;
					color[1] = int(color_bar.getFill() * 2.5);
					color[2] = 0;
				}
				if (color_bar.getFill() > 100 && color_bar.getFill() <= 200) {
					color[1] = 255;
					color[0] = 255 - int(((long int)color_bar.getFill() - 100) * 2.5);
					color[2] = 0;
				}
				if (color_bar.getFill() > 200 && color_bar.getFill() <= 300) {
					color[1] = 255;
					color[2] = int((color_bar.getFill() - 200) * 2.5);
					color[0] = 0;
				}
				if (color_bar.getFill() > 300 && color_bar.getFill() <= 400) {
					color[2] = 255;
					color[1] = 255 - int(((long int)color_bar.getFill() - 300) * 2.5);
					color[0] = 0;
				}
				if (color_bar.getFill() > 400 && color_bar.getFill() <= 500) {
					color[2] = 255;
					color[0] = int((color_bar.getFill() - 400) * 2.5);
					color[1] = 0;
				}
				if (color_bar.getFill() > 500 && color_bar.getFill() <= 600) {
					color[0] = 255;
					color[2] = 255 - int((color_bar.getFill() - 500) * 2.5);
					color[1] = 0;
				}
			}
			else {
				color_bar.setFill(600);
				editing_color = false;
			}
		SDL_Rect ColorBarFillmarker{ 700, 50 + color_bar.getFill(), 40, 4 };
		SDL_RenderFillRect(color_picker_renderer, &ColorBarFillmarker);
		SDL_SetRenderDrawColor(color_picker_renderer, final_color[0], final_color[1], final_color[2], 255);
		SDL_RenderFillRect(color_picker_renderer, &FinalColorRect);
		SDL_SetRenderDrawColor(color_picker_renderer, 255, 255, 255, 255);
		if (!editing_hue)
			SDL_RenderFillRect(color_picker_renderer, &HueMarker);
		SDL_RenderPresent(color_picker_renderer);
		SDL_Delay(1000 / 60);
	}
}
#pragma warning(pop)