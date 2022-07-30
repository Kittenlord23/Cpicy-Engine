#include "App.h"
#pragma warning(push)
#pragma warning(disable : 4838)
#pragma warning(disable: 4244)
#pragma warning(disable: 4267)
void Application::TemplateMaker() {
	SDL_Window* template_maker_window = SDL_CreateWindow("Create template", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width / 2, window_height / 2, SDL_WINDOW_SHOWN);
	SDL_Surface* template_maker_surface = SDL_GetWindowSurface(template_maker_window);
	SDL_Surface* Icon = CPU_LoadSurface(GetExecutablePath() + "Extern\\Assets\\Images\\CPU\\Images\\TemplateMakerIcon.png", template_maker_surface);
	SDL_SetWindowIcon(template_maker_window, Icon);
	SDL_FreeSurface(Icon);
	Icon = NULL;
	SDL_Renderer* template_maker_renderer = SDL_CreateRenderer(template_maker_window, -1, SDL_RENDERER_ACCELERATED);
	Mouse template_maker_mouse = Mouse();
	std::string template_user_name = "";
	std::string template_name = "File name: ";
	SDL_Texture* template_name_texture = GPU_LoadText(template_name + template_user_name + ".spice", template_maker_renderer, 18,GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	SDL_Texture* lock_template_id_texture = GPU_LoadText("Keep template ID same as template name (Recomended)", template_maker_renderer, 18,GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	std::string template_id = "";
	SDL_Texture* template_id_texture = GPU_LoadText("Template ID: " + template_id, template_maker_renderer, 18,GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	SDL_Texture* finalize_new_file = GPU_LoadText("Confirm", template_maker_renderer, 18,GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	bool editing_template_maker_name = false;
	bool editing_template_id = false;
	bool template_maker_quit = false;
	bool lock_template_id = true;
	SDL_Event e;
	while (!template_maker_quit) {
		int tpntw;
		int tpnth;
		SDL_QueryTexture(template_name_texture, NULL, NULL, &tpntw, &tpnth);
		int tittw;
		int titth;
		SDL_QueryTexture(template_id_texture, NULL, NULL, &tittw, &titth);
		template_maker_mouse.UpdateMouse();
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE) {
				if (SDL_GetWindowID(template_maker_window) == e.window.windowID)
					template_maker_quit = true;
			}
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				SDL_Rect template_text_rect{ window_width / 16, window_height / 16, window_width / 3, tpnth };
				if (IsMouseInRect(template_maker_mouse, template_text_rect)) {
					editing_template_id = false;
					if (editing_template_maker_name) {
						SDL_StopTextInput();
						editing_template_maker_name = false;
					}
					else {
						SDL_StartTextInput();
						editing_template_maker_name = true;
					}
				}
				SDL_Rect template_id_changer{ window_width / 16, window_height / 10, window_width * 0.01, window_width * 0.01 };
				if (IsMouseInRect(template_maker_mouse, template_id_changer)) {
					if (lock_template_id)
						lock_template_id = false;
					else
						lock_template_id = true;
				}
				SDL_Rect template_id_text_box{ window_width / 16, window_height / 7, window_width / 3, titth };
				if (IsMouseInRect(template_maker_mouse, template_id_text_box)) {
					editing_template_maker_name = false;
					if (editing_template_id) {
						SDL_StopTextInput();
						editing_template_id = false;
					}
					else {
						SDL_StartTextInput();
						editing_template_id = true;
					}
				}
				SDL_Rect FinalizeButton{ window_width / 2.6, window_height / 2.6,  window_width * 0.1, window_height * 0.05 };
				if (IsMouseInRect(template_maker_mouse, FinalizeButton)) {
					if (template_user_name.length() == 0) {
						MessageBoxA(NULL, "File Name must be valid", "Cpicy Engine", MB_OK | MB_ICONERROR);
					}
					else {
						if (!lock_template_id && template_id.length() == 0)
							MessageBoxA(NULL, "Templtae ID must be valid", "Cpicy Engine", MB_OK | MB_ICONERROR);
						else {
							std::ofstream File(replace(game.getWorkingPath(), split(game.getWorkingPath(), "\\").back(), "") + template_user_name + ".spice");
							if (lock_template_id) {
								File << template_user_name;
								File << " -> type_setting_vsync[True] -> type_setting_frame_delay[60] -> layers[1] -> type_text[] -> type_rect[] -> type_line[] -> type_image[] -> type_rect_wall[] -> end <->";
							}
							else {
								File << template_id;
								File << " -> type_setting_vsync[True] -> type_setting_frame_delay[60] -> layers[1] -> type_text[] -> type_rect[] -> type_line[] -> type_image[] -> type_rect_wall[] -> end <->";
							}
							File.close();
							std::ofstream EditFile(replace(game.getWorkingPath(), split(game.getWorkingPath(), "\\").back(), "") + template_user_name + ".spiceE");
							EditFile << "current id[00000000] -> end <->";
							EditFile.close();
							std::ofstream LogicFile(replace(game.getWorkingPath(), split(game.getWorkingPath(), "\\").back(), "") + template_user_name + ".spiceL");
							LogicFile.close();
							std::ofstream SourceFile(replace(game.getWorkingPath(), split(game.getWorkingPath(), "\\").back(), "") + template_user_name + ".source");
							SourceFile.close();
							std::string project_name = "File created under name: " + template_user_name;
							MessageBoxA(NULL, project_name.c_str(), "Cpicy Engine", MB_OK | MB_ICONINFORMATION);
							editing_template_maker_name = false;
							editing_template_id = false;
							template_maker_quit = true;
						}
					}
				}
			}
			if (e.type == SDL_TEXTINPUT) {
				if (!(SDL_GetModState() & KMOD_CTRL && (event.text.text[0] == 'c' || event.text.text[0] == 'C' || event.text.text[0] == 'v' || event.text.text[0] == 'V')))
				{
					if (template_user_name.length() <= 26) {
						if (editing_template_maker_name) {
							//Append character
							SDL_Color color{ 255, 255, 255, 255 };
							template_user_name += e.text.text;
							template_name_texture = GPU_LoadText(template_name + template_user_name + ".spice", template_maker_renderer, 18,GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
						}
					}
					if (template_id.length() <= 26) {
						if (editing_template_id) {
							SDL_Color color{ 255, 255, 255, 255 };
							template_id += e.text.text;
							template_id_texture = GPU_LoadText("Template ID: " + template_id, template_maker_renderer, 18,GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
						}
					}
				}
			}
			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_BACKSPACE && template_id.length() > 0) {
					if (editing_template_id) {
						SDL_Color color{ 255, 255, 255, 255 };
						template_id.pop_back();
						template_id_texture = GPU_LoadText("Template ID: " + template_id, template_maker_renderer, 18,GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
					}
				}
				if (e.key.keysym.sym == SDLK_BACKSPACE && template_user_name.length() > 0) {
					if (editing_template_maker_name) {
						SDL_Color color{ 255, 255, 255, 255 };
						template_user_name.pop_back();
						template_name_texture = GPU_LoadText(template_name + template_user_name + ".spice", template_maker_renderer, 18,GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
					}
				}
			}
		}
		SDL_QueryTexture(template_name_texture, NULL, NULL, &tpntw, &tpnth);
		SDL_Rect Background{ 0, 0, window_width / 2, window_height / 2 };
		SDL_SetRenderDrawColor(template_maker_renderer, 45, 45, 48, 255);
		SDL_RenderFillRect(template_maker_renderer, &Background);
		SDL_Rect template_text_box{ window_width / 16, window_height / 16, tpntw, tpnth };
		SDL_Rect template_text_rect{ window_width / 16, window_height / 16, window_width / 3, tpnth };
		if (!editing_template_maker_name)
			SDL_SetRenderDrawColor(template_maker_renderer, 45, 29, 45, 255);
		else
			SDL_SetRenderDrawColor(template_maker_renderer, 60, 50, 70, 255);
		SDL_RenderFillRect(template_maker_renderer, &template_text_rect);
		SDL_RenderCopy(template_maker_renderer, template_name_texture, NULL, &template_text_box);
		int tidtw;
		int tidth;
		SDL_QueryTexture(lock_template_id_texture, NULL, NULL, &tidtw, &tidth);
		SDL_Rect lock_template_name_rect{ window_width / 12, window_height / 10, tidtw, tidth };
		SDL_RenderCopy(template_maker_renderer, lock_template_id_texture, NULL, &lock_template_name_rect);
		SDL_Rect template_id_changer{ window_width / 16, window_height / 10, window_width * 0.01, window_width * 0.01 };
		SDL_SetRenderDrawColor(template_maker_renderer, 100, 100, 100, 255);
		SDL_RenderFillRect(template_maker_renderer, &template_id_changer);
		if (lock_template_id) {
			SDL_SetRenderDrawColor(template_maker_renderer, 100, 255, 100, 255);
			SDL_Rect CheckMarkBox{ (window_width / 16) + window_width * 0.002, (window_height / 10) + window_width * 0.002, window_width * 0.007, window_width * 0.007 };
			SDL_RenderFillRect(template_maker_renderer, &CheckMarkBox);
		}
		else {
			SDL_QueryTexture(template_id_texture, NULL, NULL, &tittw, &titth);
			SDL_Rect template_id_text_box{ window_width / 16, window_height / 7, window_width / 3, titth };
			SDL_Rect template_id_text{ window_width / 16, window_height / 7, tittw, titth };
			if (editing_template_id)
				SDL_SetRenderDrawColor(template_maker_renderer, 60, 50, 70, 255);
			else
				SDL_SetRenderDrawColor(template_maker_renderer, 45, 29, 45, 255);
			SDL_RenderFillRect(template_maker_renderer, &template_id_text_box);
			SDL_RenderCopy(template_maker_renderer, template_id_texture, NULL, &template_id_text);
		}
		SDL_Rect FinalizeButton{ window_width / 2.6, window_height / 2.6,  window_width * 0.08, window_height * 0.05 };
		if (IsMouseInRect(template_maker_mouse, FinalizeButton))
			SDL_SetRenderDrawColor(template_maker_renderer, 50, 255, 50, 255);
		else
			SDL_SetRenderDrawColor(template_maker_renderer, 50, 150, 50, 255);
		SDL_RenderFillRect(template_maker_renderer, &FinalizeButton);
		int fbttw;
		int fbtth;
		SDL_QueryTexture(finalize_new_file, NULL, NULL, &fbttw, &fbtth);
		SDL_Rect FinalizeText{ window_width / 2.5, window_height / 2.5, fbttw, fbtth };
		SDL_RenderCopy(template_maker_renderer, finalize_new_file, NULL, &FinalizeText);
		SDL_RenderPresent(template_maker_renderer);
		SDL_Delay(1000 / 30);
	}
	editing_template_maker_name = false;
	editing_template_id = false;
	SDL_DestroyWindow(template_maker_window);
	SDL_DestroyRenderer(template_maker_renderer);
	SDL_StopTextInput();
}
#pragma warning(pop)