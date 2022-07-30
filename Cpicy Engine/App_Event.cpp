#include "App.h"
#include "Texture.h"
#include <filesystem>
#include "Game.h"
#include "Chain.h"
#include "InputBox.h"
#include "Compiler.h"
#include "Layer.h"
#pragma warning(push)
#pragma warning(disable : 4838)
#pragma warning(disable: 4244)
#pragma warning(disable: 4267)

void Application::event_handel() {
	// Update the mouse
	mouse.UpdateMouse();

	// Event handeler

	while (SDL_PollEvent(&event) != 0)
	{
		//User requests quit
		if (event.type == SDL_QUIT)
		{
			if (window_state != SPICY_STARTUP) {
				should_quit = true;
			}
		}
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				if (window_state == SPICY_GAME) {
					window_state = SPICY_CANVAS;
				}
			}
			if (event.key.keysym.sym == SDLK_LSHIFT)
				shift_key_pressed = true;
			if (event.key.keysym.sym == SDLK_LALT)
				alt_key_pressed = true;
			if (event.key.keysym.sym == SDLK_LCTRL)
				ctrl_key_pressed = true;
			if (event.key.keysym.sym == SDLK_DELETE && ctrl_key_pressed) {
				editor.swapDeleteState(&game);
			}
			if (window_state == SPICY_STARTUP) {
				SDL_SetWindowPosition(AppWindow, 0, 0);
				SDL_SetWindowSize(AppWindow, window_width, window_height);
				AppWindowSurface = SDL_GetWindowSurface(AppWindow);
				window_state = SPICY_MAIN_MENU;
				SDL_FreeSurface(Startup480A_Surface);
				Startup480A_Surface = NULL;
			}
			if (window_state == SPICY_CREATE_NEW_PROJECT) {
				if (event.key.keysym.sym == SDLK_BACKSPACE && new_project_text.length() > 0) {
					if (editing_cpn) {
						SDL_Color color{ 255, 255, 255, 255 };
						new_project_text.pop_back();
						new_project_text_texture = GPU_LoadText(new_project_text, AppRenderer, 30,GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
					}
				}
			}
			if (window_state == SPICY_CANVAS) {
				// Keybinds and their respecitve events go here
				if (event.key.keysym.sym == SDLK_UP && ctrl_key_pressed) {
					if (editor.current_chain_id != 0) {
						editor.current_chain_id--;
					}
					
				} 
				if (event.key.keysym.sym == SDLK_DOWN && ctrl_key_pressed) {
					if (editor.current_chain_id < game.cp_EChain_data.size() + game.cp_LChain_data.size() - 1) {
						editor.current_chain_id++;
					}
				}
				if (event.key.keysym.sym == SDLK_s && ctrl_key_pressed) {
					game.saveProject();
					editor.saveFileData(replace(game.getWorkingPath(), split(game.getWorkingPath(), "\\").back(), "") + split(split(game.getWorkingPath(), "\\").back(), ".").front() + ".spiceE");
					editor.SaveLogicFile(&game);
				}
				if (event.key.keysym.sym == SDLK_n && ctrl_key_pressed) {
					TemplateMaker();
					//editor.openFiles(replace(game.getWorkingPath(), split(game.getWorkingPath(), "\\").back(), ""), AppRenderer);
				}
				if (event.key.keysym.sym == SDLK_z && ctrl_key_pressed) {
					editor.undo(&game);
				}
				if (event.key.keysym.sym == SDLK_y && ctrl_key_pressed) {
					editor.redo(&game);
				}
				if (event.key.keysym.sym == SDLK_g && ctrl_key_pressed && shift_key_pressed) {
					editor.graphics_section_showing = true;
				}
				if (event.key.keysym.sym == SDLK_c && ctrl_key_pressed && shift_key_pressed) {
					ColorPicker();
				}
				if (event.key.keysym.sym == SDLK_d && ctrl_key_pressed) {
					editor.swapCameraRelation();
				}
				if (event.key.keysym.sym == SDLK_o && ctrl_key_pressed) {
					editor.importFont(&game);
				}
				if (editor.text_editor_section_showing) {
					if (event.key.keysym.sym == SDLK_BACKSPACE) {

						if (editor.cursor_spacing != -1 && editor.lines.size() != 0) {
							editor.lines[editor.cursor_line_number].erase(editor.lines[editor.cursor_line_number].begin() + editor.cursor_spacing);
							editor.cursor_spacing--;
						}
						else {
							if (editor.lines.size() == 0) {
								editor.lines.push_back("");
							}
							else {
								if (editor.cursor_line_number != 0) {
									editor.cursor_spacing = editor.lines[editor.cursor_line_number - 1].size() - 1;
									editor.lines[editor.cursor_line_number - 1] += editor.lines[editor.cursor_line_number];
									editor.lines.erase(editor.lines.begin() + editor.cursor_line_number);
									editor.cursor_line_number--;
								}
							}
						}

					}
					if (event.key.keysym.sym == SDLK_v && ctrl_key_pressed) {
						editor.lines[editor.cursor_line_number].insert(editor.cursor_spacing + 2, GetClipboardData());
						editor.cursor_spacing += GetClipboardData().size() - 1;
					}
					if (event.key.keysym.sym == SDLK_RETURN) {
						editor.cursor_line_number++;
						editor.lines.insert(editor.lines.begin() + editor.cursor_line_number, "");
						std::string t = editor.lines[editor.cursor_line_number - 1].substr(editor.cursor_spacing+1);
						editor.lines[editor.cursor_line_number - 1] = editor.lines[editor.cursor_line_number - 1].substr(0, find_last_index(editor.lines[editor.cursor_line_number - 1], t));
						editor.lines[editor.cursor_line_number].append(t);
						editor.cursor_spacing = -1;
					}
					if (event.key.keysym.sym == SDLK_TAB) {
						editor.lines[editor.cursor_line_number].insert(editor.cursor_spacing + 1, TAB);
						editor.cursor_spacing += 8;
					}
					if (event.key.keysym.sym == SDLK_t && ctrl_key_pressed) {
						compile(editor.lines, &game, &editor.errors);
					}
				}
			}
			if (window_state == SPICY_GAME) {

			}
		}
		if (event.type == SDL_KEYUP) {
			if (event.key.keysym.sym == SDLK_LSHIFT)
				shift_key_pressed = false;
			if (event.key.keysym.sym == SDLK_LALT)
				alt_key_pressed = false;
			if (event.key.keysym.sym == SDLK_LCTRL)
				ctrl_key_pressed = false;
			if (window_state == SPICY_CANVAS && editor.text_editor_section_showing) {
				
			}
		}

		// This is used for typing the name of the new
		// project

		if (event.type == SDL_TEXTINPUT) {
			if (editor.text_editor_section_showing) {
				if (!(SDL_GetModState() & KMOD_CTRL && (event.text.text[0] == 'c' || event.text.text[0] == 'C' || event.text.text[0] == 'v' || event.text.text[0] == 'V'))) {
					if (editor.lines.size() == 0) {
						editor.lines.push_back(" ");
					}
					editor.lines[editor.cursor_line_number].insert(editor.cursor_spacing + 1, event.text.text);
					editor.cursor_spacing++;
				}
			}
			if (!(SDL_GetModState() & KMOD_CTRL && (event.text.text[0] == 'c' || event.text.text[0] == 'C' || event.text.text[0] == 'v' || event.text.text[0] == 'V')))
			{
				if (new_project_text.length() <= 26) {
					if (editing_cpn) {
						//Append character
						SDL_Color color{ 255, 255, 255, 255 };
						new_project_text += event.text.text;
						new_project_text_texture = GPU_LoadText(new_project_text, AppRenderer, 30,GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
					}
				}
			}
		}

		if (event.type == SDL_MOUSEBUTTONDOWN) {
			mouse.poll_button_presses(event, true);
			if (window_state == SPICY_MAIN_MENU) {
				SDL_Rect NewProjectRect{ window_width * 0.25, window_height * 0.05, window_width * 0.2, window_height * 0.1 };
				if (IsMouseInRect(mouse, NewProjectRect)) {
					window_state = SPICY_CREATE_NEW_PROJECT; 
				}
				SDL_Rect SelectProjectRect{ window_width * 0.03, window_height * 0.05, window_width * 0.2, window_height * 0.1 };
				if (IsMouseInRect(mouse, SelectProjectRect)) {
					// Logic for proccessing valid projects, and safely moving to the editor state

					// Function is defined in Kiran.h, creates a file explorer diaolog box, and returns
					// a string version of it's path

					std::string try_path = GetFilePath();
					try {
						// Will throw an error if they didn't do anything

						if (try_path.substr(try_path.find_last_of(".")) == ".spiceP") {
							// If it is, set the current game path to it.

							if (game.setWorkingPath(replace(try_path, split(try_path, "\\").back(), "") + "main.spice", AppRenderer)) {
								editor.reciveFileData(replace(game.getWorkingPath(), split(game.getWorkingPath(), "\\").back(), "") + split(split(game.getWorkingPath(), "\\").back(), ".").front() + ".spiceE");
								window_state = SPICY_CANVAS;
								for (int t = 0; t < game.loaded_textures.size(); t++) {
									editor.view_image_textures.push_back(GPU_LoadText(game.loaded_textures[t].name, AppRenderer, floor(GetDesktopResolution('y') / 77),GetExecutablePath() + "Extern\\Fonts\\arial.ttf"));
								}
								editor.openFiles(replace(game.getWorkingPath(), split(game.getWorkingPath(), "\\").back(), ""), AppRenderer);
								editor.LoadLogicFile(&game);
								mouse.x = -2;
								mouse.y = -2;
							}
							else
								window_state = SPICY_MAIN_MENU;
						}
						else {
							// If not, tell the user they are dumb dumb

							MessageBoxA(NULL, "File is not valied:\nPlease choose a spicy project (.spice)", "Cpicy Engine", MB_OK | MB_ICONEXCLAMATION);
						}
					}
					// If they didn't do anything, catch the error
					catch (std::out_of_range) {}
				}
			}
			if (window_state == SPICY_CREATE_NEW_PROJECT) {
				SDL_Rect SideScrollTemplateBG{ window_width * 0.11, window_height * 0.4, window_width * 0.08, window_height * 0.08 };
				if (IsMouseInRect(mouse, SideScrollTemplateBG)) {
					editor.project_template_mode = ProjectTemplateMode::SideScroller;
				}
				SDL_Rect NewProjectNameTextRect{ window_width * 0.1, window_height * 0.2, window_width * 0.4, window_height * 0.1 };
				if (IsMouseInRect(mouse, NewProjectNameTextRect)) {
					if (editing_cpn) {
						editing_cpn = false;
						SDL_StopTextInput();
					}
					else {
						SDL_StartTextInput();
						editing_cpn = true;
					}
				}
				SDL_Rect BackButton{ window_width * 0.55, window_height * 0.86, window_width * 0.2, window_height * 0.1 };
				if (IsMouseInRect(mouse, BackButton)) {
					window_state = SPICY_MAIN_MENU;
					editing_cpn = false;
				}
				SDL_Rect FinalizeProjectRect{ window_width * 0.78, window_height * 0.86, window_width * 0.2, window_height * 0.1 };
				if (IsMouseInRect(mouse, FinalizeProjectRect)) {
					std::string name = "New project created under name: " + new_project_text;
					std::string dir_name = "Projects\\";
					dir_name.append(new_project_text);
					if (MakeDir(dir_name.c_str())) {
						std::ofstream project_file("Projects\\" + new_project_text + "\\" + new_project_text + ".spiceP");
						project_file.close();
						std::ofstream file("Projects\\" + new_project_text + "\\" + "main.spice");
						file << "default -> type_setting_vsync[True] -> type_setting_frame_delay[60] -> layers[1] -> type_text[] -> type_rect[] -> type_line[] -> type_image[] -> type_rect_wall[] -> end <->";
						file.close();
						std::ofstream EditFile("Projects\\" + new_project_text + "\\" + "main.spiceE");
						EditFile << "current id[00000000] -> end <->";
						EditFile.close();
						std::ofstream LogicFile("Projects\\" + new_project_text + "\\" +  "main.spiceL");

						LogicFile.close();
						std::ofstream LogicSourceFile("Projects\\" + new_project_text + "\\" + "main.source");
						LogicSourceFile << "On Start:\n        set camera speed: 1\n        enable gravity\nOn Event:\n        When: key press[d]\n                start right camera\n        When: key release[d]\n                stop right camera\n        When: key press[a]\n                start left camera\n        When: key release[a]\n                stop left camera\n        When: key press[w]\n                start up camera\nOn Loop:\n";
						LogicSourceFile.close();
						MakeDir((dir_name + "\\Images").c_str());
						MakeDir((dir_name + "\\Sounds").c_str());
						MakeDir((dir_name + "\\Fonts").c_str());
						MessageBoxA(NULL, name.c_str(), "Cpicy Engine", MB_OK | MB_ICONINFORMATION);
						window_state = SPICY_MAIN_MENU;
					}
					else {
						MessageBoxA(NULL, "Failed to make project, make sure the project name is valid and does not allreadry exist", "Cpicy Engine", MB_OK | MB_ICONERROR);
					}
				}
			} 
			if (window_state == SPICY_CANVAS) {
				if (editor.text_editor_section_showing) {
					auto temp_line_number = editor.line_number;
					auto temp_line_spacing = editor.line_spacing;
					editor.line_number = 0;
					editor.line_spacing = 0;
					bool flag = 0;
					for (int x = 0; x < editor.lines.size(); x++) {
						for (int y = 0; y < editor.lines[x].size(); y++) {
							SDL_Rect background{ window_width * 0.15, window_height * 0.8, window_width * 0.85, window_height * 0.2 };
							if (editor.lines[x].size() != 0) {
								int w;
								int h;
								SDL_QueryTexture(editor.Alphabet[y], NULL, NULL, &w, &h);
								SDL_Rect character{ window_width * 0.2 +  editor.line_spacing * 15 + editor.editor_camera.x, window_width * 0.2 +  editor.line_number * 24 + editor.editor_camera.y, w / 2, h };
								if (editor.show_error_section) {
									if (!IsMouseInRect(mouse, background)) {
										if (IsMouseInRect(mouse, character)) {
											editor.cursor_line_number = editor.line_number;
											editor.cursor_spacing = editor.line_spacing - 1;
											flag = true;
										}
										character.x += w / 2;
										if (IsMouseInRect(mouse, character)) {
											editor.cursor_line_number = editor.line_number;
											editor.cursor_spacing = editor.line_spacing;
											flag = true;
										}
									}
								}
								else {
									if (IsMouseInRect(mouse, character)) {
										editor.cursor_line_number = editor.line_number;
										editor.cursor_spacing = editor.line_spacing - 1;
										flag = true;
									}
									character.x += w / 2;
									if (IsMouseInRect(mouse, character)) {
										editor.cursor_line_number = editor.line_number;
										editor.cursor_spacing = editor.line_spacing;
										flag = true;
									}
								}
								editor.line_spacing++;
								if (!flag) {
									if (editor.show_error_section) {
										if (!IsMouseInRect(mouse, background)) {
											editor.editing_editor_text = false;
											SDL_StopTextInput();
										}
									}
									else {
										editor.editing_editor_text = false;
										SDL_StopTextInput();
									}
								}
								else {
									if (editor.show_error_section) {
										if (!IsMouseInRect(mouse, background)) {
											editor.editing_editor_text = true;
											SDL_StartTextInput();
										}
									}
									else {
										editor.editing_editor_text = false;
										SDL_StopTextInput();
									}
								}
							}
						}
						if (editor.lines[x].size() == 0) {
							int w;
							int h;
							SDL_QueryTexture(editor.Alphabet[0], NULL, NULL, &w, &h);
							SDL_Rect character{ window_width * 0.2 + editor.line_spacing * 15 + editor.editor_camera.x, window_width * 0.2 + editor.line_number * 24 + editor.editor_camera.y, 2000, h };
							if (IsMouseInRect(mouse, character)) {
								editor.cursor_line_number = editor.line_number;
								editor.cursor_spacing = -1;
								flag = true;
							}
							editor.line_spacing = 0;
							if (!flag) {
								editor.editing_editor_text = false;
								SDL_StopTextInput();
							}
							else {
								editor.editing_editor_text = true;
								SDL_StartTextInput();
							}
						}
						editor.line_number++;
						editor.line_spacing = 0;
					}

					editor.line_number = temp_line_number;
					editor.line_spacing = temp_line_spacing;
				}
				if (editor.deleting_object && !editor.text_editor_section_showing) {
					if (Viewer.rectangles) {
						int x = 0;
						for (auto r : game.cp_rect_data) {
							SDL_Rect rect{ r.x + window_width * 0.15 + editor.editor_camera.x, r.y + window_height * 0.2 + editor.editor_camera.y, r.w, r.h };
							if (IsMouseInRect(mouse, rect)) {
								game.cp_rect_data.erase(game.cp_rect_data.begin() + x);
							}
							x++;
						}
					}
					if (Viewer.rect_walls) {
						int x = 0;
						for (auto r : game.cp_rect_wall_data) {
							SDL_Rect rect{ r.x + window_width * 0.15 + editor.editor_camera.x, r.y + window_height * 0.2 + editor.editor_camera.y, r.w, r.h };
							if (IsMouseInRect(mouse, rect)) {
								game.cp_rect_wall_data.erase(game.cp_rect_wall_data.begin() + x);
							}
							x++;
						}
					}
					if (Viewer.textures) {
						int x = 0;
						for (auto r : game.cp_texture_data) {
							SDL_Rect rect{ r.x + window_width * 0.15 + editor.editor_camera.x, r.y + window_height * 0.2 + editor.editor_camera.y, r.w, r.h };
							if (IsMouseInRect(mouse, rect)) {
								game.cp_texture_data.erase(game.cp_texture_data.begin() + x);
							}
							x++;
						} 
					}
				}
				for (int x = 0; x < ribbons.size(); x++) {
					if ((ribbons[x])->active) {
						if (x == 0) { // HOME
							SDL_Rect HomeLabelRect{ window_width * 0.05, window_height * 0.07, window_width * 0.0425, window_height * 0.07 };
							if (IsMouseInRect(mouse, HomeLabelRect)) {
								game.saveProject();
								editor.saveFileData(replace(game.getWorkingPath(), split(game.getWorkingPath(), "\\").back(), "") + split(split(game.getWorkingPath(), "\\").back(), ".").front() + ".spiceE");
								editor.SaveLogicFile(&game);
								window_state = SPICY_MAIN_MENU;
							}
							SDL_Rect DocuLabelRect{ window_width * 0.1125, window_height * 0.07, window_width * 0.0425 * 1.25, window_height * 0.07 }; // WIDTH: + 0.02
							if (IsMouseInRect(mouse, DocuLabelRect)) {
								std::string path = GetExecutablePath() + "Extern\\Help\\Docu.html";
								ShellExecute(NULL, "open", path.c_str(),
									NULL, NULL, SW_SHOWNORMAL);
							}
						}
						else if (x == 1) { // GRAPHICS SECTION
							SDL_Rect RectangleLabelRect{ window_width * 0.05, window_height * 0.07, window_width * 0.0425, window_height * 0.07 };
							if (IsMouseInRect(mouse, RectangleLabelRect)) {
								if (editor.creating_rectangle) {
									editor.creating_rectangle = false;
								}
								else {
									editor.creating_image = false;
									editor.creating_rectangle = true;
									editor.creating_rect_wall = false;
								}
							}
							SDL_Rect ImageLabelRect{ window_width * 0.12, window_height * 0.07, window_width * 0.0425, window_height * 0.07 };
							if (IsMouseInRect(mouse, ImageLabelRect)) {
								if (editor.creating_image) {
									editor.creating_image = false;
								}
								else {
									editor.creating_image = true;
									editor.creating_rectangle = false;
									editor.creating_rect_wall = false;
								}
							}
							SDL_Rect PrRectangleLabelRect{ window_width * 0.19, window_height * 0.07, window_width * 0.0425, window_height * 0.07 };
							if (IsMouseInRect(mouse, PrRectangleLabelRect)) {
								InputBox i = InputBox::InputBox(600, 150, "Add Rectangle", "Add Rectangle", 30, true, false, false, "x:y:w:h:id");
								if (i.GetInputFromBox() == "x:y:w:h:id") {

								}
								else {
									editor.cmdQueue.push_queue("Add Rectangle: " + i.GetInputFromBox());
									editor.cmdQueue.flush_queue(&this->game, &this->editor);
								}
							}
							SDL_Rect PrTextureLabelRect{ window_width * 0.26, window_height * 0.07, window_width * 0.0425, window_height * 0.07 };
							if (IsMouseInRect(mouse, PrTextureLabelRect)) {
								InputBox i = InputBox::InputBox(600, 150, "Add Image", "Add Image", 30, true, false, false, "x:y:w:h:rotation_angle:id");
								if (i.GetInputFromBox() == "x:y:w:h:rotation_angle:id") {

								}
								else {
									editor.cmdQueue.push_queue("Add Image: " + i.GetInputFromBox());
									editor.cmdQueue.flush_queue(&this->game, &this->editor);
								}
							}
						}
						else if (x == 2) { // INTERACTIVE
							SDL_Rect RectangleWallLabelRect{ window_width * 0.05, window_height * 0.07, window_width * 0.0425, window_height * 0.07 };
							if (IsMouseInRect(mouse, RectangleWallLabelRect)) {
								if (editor.creating_rect_wall) {
									editor.creating_rect_wall = false;
								}
								else {
									editor.creating_image = false;
									editor.creating_rectangle = false;
									editor.creating_rect_wall = true;
								}
							}
						}
					}
				}
				SDL_Rect StartButton{ window_width * 0.93, window_height * 0.17, window_width * 0.1, window_height * 0.03 };
				{
					// Unscope this after
					SDL_Rect BG{ window_width * 0.015, window_height * 0.016, window_width * 0.2, window_height * 0.1 };
					if (IsMouseInRect(mouse, StartButton) && !editor.file_section_showing) {
						system("cls");
						window_state = SPICY_GAME;
						editor.preview_game = game;
						editor.preview_game.game_camera.x = 0;
						editor.preview_game.game_camera.y = 0;
						editor.preview_game.Sort();
						editor.preview_game.recersion_count = 0;
						if (game.enable_errors) {
							::ShowWindow(GetConsoleWindow(), SW_SHOW);
						}
						if (editor.preview_game.start_chain.chain.size() != 0) {
							editor.preview_game.start_chain.chain.front().trigger.cause = "start chain";
							editor.preview_game.start_chain.InterpretChain(event, &editor.preview_game, clock, AppRenderer);
						}
					}
				}
				SDL_Rect DropDownBar{ 0, window_height * 0.2, window_width * 0.01, window_height * 0.8 };
				if (IsMouseInRect(mouse, DropDownBar)) {
					editor.moving_DropDownBar = true;
				}
				int drop_down_buffer = 0;
				if (editor.graphics_section_showing) {
					drop_down_buffer += window_height * 0.365;
				}
				for (int x = 0; x < ribbons.size(); x++) {
					if (ribbons[x]->update(mouse, x)) {
						for (int y = 0; y < ribbons.size(); y++) {
							ribbons[y]->active = false;
						}
						ribbons[x]->active = true;
					}
				}
				SDL_Rect Canvas{ window_width * 0.15, window_height * 0.2, window_width * 0.85, window_height * 0.8 };
				if (IsMouseInRect(mouse, Canvas)) {
					editor.moving_camera = true;
				}
				SDL_Rect CameraRelationButton{ 0, (window_height * 0.168) + 3, window_width * 0.15, window_height * 0.03 };
				if (IsMouseInRect(mouse, CameraRelationButton)) {
					if (editor.creation_dynamic_state == SPICY_STATIC) {
						editor.creation_dynamic_state = SPICY_DYNAMIC;
						DYNAMICSTATICSTATE_Texture = GPU_LoadText("Moves With Camera: YES", AppRenderer, floor(GetDesktopResolution('y') / 50),GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
					}
					else {
						editor.creation_dynamic_state = SPICY_STATIC;
						DYNAMICSTATICSTATE_Texture = GPU_LoadText("Moves With Camera: NO", AppRenderer, floor(GetDesktopResolution('y') / 50),GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
					}
				}
				int buffer = 0;
				SDL_Rect _FileSectionRect{ window_width * 0.005, window_height * 0.218 - editor.DropDownBar.getFill(), window_width * 0.145, window_height * 0.015 };
				if (IsMouseInRect(mouse, _FileSectionRect)) {
					if (editor.collapse_file_section) {
						editor.collapse_file_section = false;
					}
					else {
						editor.collapse_file_section = true;
					}
				}
				for (int x = 0; x < editor.view_file_textures.size(); x++) {
					SDL_Rect TextGoesHere{ window_width * 0.044, window_height * 0.235 + (window_height * x / 50) - editor.DropDownBar.getFill(), window_width * 0.1, window_height * 0.02 };
					buffer += (window_height * x / 100);
					if (IsMouseInRect(mouse, TextGoesHere)) {
						// assertion failed here, at some point in the code the path part of the return value of getWorkingPath fails,
						// so check to see if it happens by using a : since the path has to include the drive (C:Users...)

						if (editor.file_paths[x].find(":") == std::string::npos) {
							if (game.getWorkingPath() != editor.file_paths[x]) {
								game.saveProject();
								editor.saveFileData(replace(game.getWorkingPath(), split(game.getWorkingPath(), "\\").back(), "") + split(split(game.getWorkingPath(), "\\").back(), ".").front() + ".spiceE");
								editor.SaveLogicFile(&game);
								game.setWorkingPath(replace(game.getWorkingPath(), split(game.getWorkingPath(), "\\").back(), "") + editor.file_paths[x], AppRenderer);
								editor.LoadLogicFile(&game);
								editor.reciveFileData(replace(game.getWorkingPath(), split(game.getWorkingPath(), "\\").back(), "") + split(split(game.getWorkingPath(), "\\").back(), ".").front() + ".spiceE");
							}
						}
						else {
							if (game.getWorkingPath() != editor.file_paths[x]) {
								game.saveProject();
								editor.saveFileData(replace(game.getWorkingPath(), split(game.getWorkingPath(), "\\").back(), "") + split(split(game.getWorkingPath(), "\\").back(), ".").front() + ".spiceE");
								editor.SaveLogicFile(&game);
								game.setWorkingPath(replace(game.getWorkingPath(), split(game.getWorkingPath(), "\\").back(), "") + editor.file_paths[x], AppRenderer);
								editor.LoadLogicFile(&game);
								editor.reciveFileData(replace(game.getWorkingPath(), split(game.getWorkingPath(), "\\").back(), "") + split(split(game.getWorkingPath(), "\\").back(), ".").front() + ".spiceE");
							}
						}
					}
				}
				buffer += (window_height * 0.05);
				SDL_Rect _ImageSectionRect{ window_width * 0.005, window_height * 0.218 + buffer - editor.DropDownBar.getFill(), window_width * 0.145, window_height * 0.015 };
				if (IsMouseInRect(mouse, _ImageSectionRect)) {
					if (editor.collapse_image_section) {
						editor.collapse_image_section = false;
					}
					else {
						editor.collapse_image_section = true;
					}
				}
				for (int x = 0; x < editor.view_image_textures.size(); x++) {
					SDL_Rect TextGoesHere{ window_width * 0.044, window_height * 0.235 + buffer + (window_height * x / 50) - editor.DropDownBar.getFill(), window_width * 0.1, window_height * 0.02 };
					buffer += (window_height * x / 100);
					if (IsMouseInRect(mouse, TextGoesHere)) {
						editor.current_texture.content = &game.loaded_textures[x].content;
						editor.current_texture.name = game.loaded_textures[x].name;
						CTEXTUREUI_Texture = GPU_LoadText("Current image: " + game.loaded_textures[x].name, AppRenderer, floor(GetDesktopResolution('y') / 50), GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
						editor.view_image_index = x;
					}
				}				
			}
		}
		if (event.type == SDL_MOUSEBUTTONUP) {
			SDL_Rect AboutIconRect{ window_width * 0.95, 0, window_width * 0.01, window_height * 0.02 };
			if (IsMouseInRect(mouse, AboutIconRect)) {
				// AboutPage();
			}
			if (window_state != SPICY_STARTUP) {
				if (mouse.x > window_width * 0.96) {
					if (mouse.y < window_height * 0.02) {
						if (window_state != SPICY_CANVAS)
							should_quit = true;
						else {
							auto QuitState = MessageBoxA(NULL, "Are you sure you want to quit?\nYour files will automaticly be saved", "Cpicy Engine", MB_YESNO | MB_ICONINFORMATION);
							if (QuitState == 6) {
								game.saveProject();
								editor.saveFileData(replace(game.getWorkingPath(), split(game.getWorkingPath(), "\\").back(), "") + split(split(game.getWorkingPath(), "\\").back(), ".").front() + ".spiceE");
								editor.SaveLogicFile(&game);
								should_quit = true;
							}
						}
					}
				}
			}
			if (window_state == SPICY_CANVAS) {
				SDL_Rect SwapDevModeRect{ window_width * 0.95, window_height * 0.2, window_width * 0.05, window_width * 0.05 };
				if (IsMouseInRect(mouse, SwapDevModeRect)) {
					if (editor.text_editor_section_showing) {
						editor.text_editor_section_showing = false;
					}
					else {
						editor.text_editor_section_showing = true;
						editor.creating_rectangle = false;
						editor.creating_rect_wall = false;
						editor.creating_image = false;
					}
				}
				editor.moving_camera = false;
				SDL_Rect DropDownBar{ 0, window_height * 0.2, window_width * 0.01, window_height * 0.8 };
				if (IsMouseInRect(mouse, DropDownBar)) {
					editor.moving_DropDownBar = false;
				}
				SDL_Rect Canvas{ window_width * 0.15, window_height * 0.2, window_width * 0.85, window_height * 0.8 };
				if (IsMouseInRect(mouse, Canvas)) {
					if (editor.creating_rectangle) {
						if (editor.creation_points.size() == 0 && !ctrl_key_pressed) {
							Point point;
							point.x = mouse.x - window_width * 0.15 - editor.editor_camera.x;
							point.y = mouse.y - window_height * 0.2 - editor.editor_camera.y;
							editor.creation_points.push_back(point);
						}
						else if (editor.creation_points.size() == 1 && !ctrl_key_pressed) {
							Point point;
							point.x = mouse.x - window_width * 0.15 - editor.editor_camera.x;
							point.y = mouse.y - window_height * 0.2 - editor.editor_camera.y;
							editor.creation_points.push_back(point);
							if (editor.creation_points[0].x > editor.creation_points[1].x) {
								std::swap(editor.creation_points[0].x, editor.creation_points[1].x);
							}
							if (editor.creation_points[0].y > editor.creation_points[1].y) {
								std::swap(editor.creation_points[0].y, editor.creation_points[1].y);
							}
							GRect rect{ editor.creation_points[0].x, editor.creation_points[0].y, editor.creation_points[1].x - editor.creation_points[0].x, editor.creation_points[1].y - editor.creation_points[0].y, editor.color[0], editor.color[1] , editor.color[2],  editor.creation_dynamic_state, editor.current_id, editor.current_layer_number, 0 };
							editor.undo_stack.push_back("rectangle");
							editor.updateRedoStack();
							editor.creation_points.clear();
							game.addRect(rect);
							Graphic add_t;
							add_t.gtype = 0;
							add_t.content.rect = rect;
							game.layers[editor.current_layer_number - 1].chain.push_back(add_t);
							editor.incrementIdValue();
						}
					}
					if (editor.creating_image) {
						if (editor.creation_points.size() == 0 && !ctrl_key_pressed) {
							Point point;
							point.x = mouse.x - window_width * 0.15 - editor.editor_camera.x;
							point.y = mouse.y - window_height * 0.2 - editor.editor_camera.y;
							editor.creation_points.push_back(point);
						}
						else if (editor.creation_points.size() == 1 && !ctrl_key_pressed) {
							Point point;
							point.x = mouse.x - window_width * 0.15 - editor.editor_camera.x;
							point.y = mouse.y - window_height * 0.2 - editor.editor_camera.y;
							editor.creation_points.push_back(point);
							if (editor.creation_points[0].x > editor.creation_points[1].x) {
								std::swap(editor.creation_points[0].x, editor.creation_points[1].x);
							}
							if (editor.creation_points[0].y > editor.creation_points[1].y) {
								std::swap(editor.creation_points[0].y, editor.creation_points[1].y);
							}
						}
						else if (editor.creation_points.size() == 2 && !ctrl_key_pressed) {
							Point point;
							point.x = mouse.x - window_width * 0.15 - editor.editor_camera.x;
							point.y = mouse.y - window_height * 0.2 - editor.editor_camera.y;
							editor.creation_points.push_back(point);

							GTexture texture{ editor.creation_points[0].x, editor.creation_points[0].y, editor.creation_points[1].x - editor.creation_points[0].x, editor.creation_points[1].y - editor.creation_points[0].y, editor.creation_points[1].x - editor.creation_points[2].x, editor.creation_dynamic_state, editor.current_texture, editor.current_id, editor.current_layer_number, 0 };
							game.addTexture(texture);
							Graphic add_t;
							add_t.gtype = 1;
							add_t.content.texture = texture;
							game.layers[editor.current_layer_number - 1].chain.push_back(add_t);
							editor.undo_stack.push_back("texture");
							editor.updateRedoStack();
							editor.creation_points.clear();
							editor.incrementIdValue();
						}
					}
					if (editor.creating_rect_wall) {
						if (editor.creation_points.size() == 0 && !ctrl_key_pressed) {
							Point point;
							point.x = mouse.x - window_width * 0.15 - editor.editor_camera.x;
							point.y = mouse.y - window_height * 0.2 - editor.editor_camera.y;
							editor.creation_points.push_back(point);
						}
						else if (editor.creation_points.size() == 1 && !ctrl_key_pressed) {
							Point point;
							point.x = mouse.x - window_width * 0.15 - editor.editor_camera.x;
							point.y = mouse.y - window_height * 0.2 - editor.editor_camera.y;
							editor.creation_points.push_back(point);
							if (editor.creation_points[0].x > editor.creation_points[1].x) {
								std::swap(editor.creation_points[0].x, editor.creation_points[1].x);
							}
							if (editor.creation_points[0].y > editor.creation_points[1].y) {
								std::swap(editor.creation_points[0].y, editor.creation_points[1].y);
							}
							GRWall wall{ editor.creation_points[0].x, editor.creation_points[0].y, editor.creation_points[1].x - editor.creation_points[0].x, editor.creation_points[1].y - editor.creation_points[0].y, editor.creation_dynamic_state, editor.current_id };
							editor.undo_stack.push_back("rect wall");
							editor.updateRedoStack();
							editor.creation_points.clear();
							game.cp_rect_wall_data.push_back(wall);
							editor.incrementIdValue();
						}
					}
				}
				else {
					editor.creation_points.clear();
				}
			}
			mouse.poll_button_presses(event, false);
		}

		if (window_state == SPICY_GAME) {
			for (int x = 0; x < editor.preview_game.cp_EChain_data.size(); x++) {
				editor.preview_game.cp_EChain_data[x].InterpretChain(event, &editor.preview_game, this->clock, AppRenderer);
				editor.preview_game.recersion_count = 0;
			}
		}
	}
}

#pragma warning (pop)