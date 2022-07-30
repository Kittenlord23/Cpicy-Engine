#include "App.h"
#include "vec2.h"
#include <cmath>
#pragma warning(push)
#pragma warning(disable : 4838)
#pragma warning(disable: 4244)
#pragma warning(disable: 4267)

constexpr auto ribbon_home = 0;
constexpr auto ribbon_graphic = 1;
constexpr auto ribbon_interactive = 2;
constexpr auto ribbon_layer = 3;
constexpr auto ribbon_edit = 4;

void Application::loop() {
	clock.current = SDL_GetTicks();
	clock.deltaTime = (clock.current - clock.lastupdate);
	event_handel();
	if (window_state == SPICY_STARTUP) {
		// ::ShowWindow(GetConsoleWindow(), SW_HIDE);
		SDL_FillRect(AppWindowSurface, NULL, 0);
		SDL_BlitSurface(Startup480A_Surface, NULL, AppWindowSurface, NULL);
		SDL_UpdateWindowSurface(AppWindow);
	}
	if (window_state == SPICY_CREATE_NEW_PROJECT) {
		SDL_RenderClear(AppRenderer);
		SDL_Rect Background{ 0,0, window_width, window_height };
		SDL_SetRenderDrawColor(AppRenderer, 0x2D, 0x2D, 0x30, 255);
		SDL_RenderFillRect(AppRenderer, &Background);
		SDL_Rect Foreground{ 0, window_height * 0.2, window_width, window_height * 0.8 };
		SDL_SetRenderDrawColor(AppRenderer, 40, 40, 43, 255);
		SDL_RenderFillRect(AppRenderer, &Foreground);
		SDL_Rect AppBar{ 0, 0, window_width, window_height * 0.02 };
		SDL_SetRenderDrawColor(AppRenderer, 0x5A, 0x5A, 0x5A, 255);
		SDL_RenderFillRect(AppRenderer, &AppBar);
		SDL_Rect NewProjectRectText{ window_width * 0.11, window_height * 0.078, window_width * 0.18, window_height * 0.045 };
		SDL_RenderCopy(AppRenderer, CP1_Texture, NULL, &NewProjectRectText);
		SDL_Rect QuitButtonRect{ window_width * 0.96, 0, window_width * 0.04, window_height * 0.02 };
		if (mouse.x > window_width * 0.96) {
			if (mouse.y < window_height * 0.02) {
				SDL_SetRenderDrawBlendMode(AppRenderer, SDL_BLENDMODE_ADD);
				SDL_RenderCopy(AppRenderer, QuitButtonTexture, NULL, &QuitButtonRect);
				SDL_SetRenderDrawColor(AppRenderer, 255, 50, 50, 150);
				SDL_RenderFillRect(AppRenderer, &QuitButtonRect);
				SDL_SetRenderDrawBlendMode(AppRenderer, SDL_BLENDMODE_NONE);
			}
			else {
				SDL_RenderCopy(AppRenderer, QuitButtonTexture, NULL, &QuitButtonRect);
			}
		}
		else {
			SDL_RenderCopy(AppRenderer, QuitButtonTexture, NULL, &QuitButtonRect);
		}
		SDL_SetRenderDrawColor(AppRenderer, 45, 29, 48, 255);
		SDL_Rect NewProjectNameRect{ window_width * 0.1, window_height * 0.2, window_width * 0.4, window_height * 0.1 };
		SDL_RenderFillRect(AppRenderer, &NewProjectNameRect);
		SDL_Rect NewProjectNameRectText{ window_width * 0.11, window_height * 0.226, window_width * 0.103, window_height * 0.037 };
		SDL_RenderCopy(AppRenderer, NP1_Texture, NULL, &NewProjectNameRectText);
		int nptt_width;
		int nptt_height;
		SDL_QueryTexture(new_project_text_texture, NULL, NULL, &nptt_width, &nptt_height);
		SDL_Rect TextInputField{ window_width * 0.22, window_height * 0.23, nptt_width, nptt_height };
		SDL_RenderCopy(AppRenderer, new_project_text_texture, NULL, &TextInputField);
		if (editing_cpn) {
			SDL_SetRenderDrawColor(AppRenderer, 125, 125, 125, 255);
			SDL_Rect Cursor{ window_width * 0.22 + nptt_width, window_height * 0.23, window_width * 0.002, nptt_height };
			SDL_RenderFillRect(AppRenderer, &Cursor);
		}
		SDL_Rect FinalizeProjectRect{ window_width * 0.78, window_height * 0.861, window_width * 0.2, window_height * 0.1 };
		SDL_SetRenderDrawColor(AppRenderer, 45, 29, 45, 255);
		SDL_RenderFillRect(AppRenderer, &FinalizeProjectRect);
		SDL_Rect FinalizeProjectRectText{ window_width * 0.841, window_height * 0.887, window_width * 0.062, window_height * 0.044 };
		SDL_RenderCopy(AppRenderer, CP2_Texture, NULL, &FinalizeProjectRectText);
		SDL_Rect BackButton{ window_width * 0.55, window_height * 0.86, window_width * 0.2, window_height * 0.1 };
		SDL_Rect BackButtonText{ window_width * 0.621, window_height * 0.887, window_width * 0.05, window_height * 0.044 };
		SDL_RenderFillRect(AppRenderer, &BackButton);
		SDL_RenderCopy(AppRenderer, BP1_Texture, NULL, &BackButtonText);
		SDL_Rect SideScrollTemplateBG{ window_width * 0.11, window_height * 0.4, window_width * 0.08, window_height * 0.1 };
		SDL_Rect SideScrollTemplateIcon{ window_width * 0.11, window_height * 0.42, window_width * 0.08, window_height * 0.08 };
		int nptttw;
		int npttth;
		SDL_QueryTexture(SIDESCROLLTEXT_Texture, NULL, NULL, &nptttw, &npttth);
		SDL_Rect SideScrollTextRect{ window_width * 0.15 - nptttw / 2, window_height * 0.4, nptttw, npttth };
		if (IsMouseInRect(mouse, SideScrollTemplateBG) || editor.project_template_mode == ProjectTemplateMode::SideScroller) {
			SDL_SetRenderDrawColor(AppRenderer, 200, 200, 200, 225);
			SDL_RenderFillRect(AppRenderer, &SideScrollTemplateBG);
		}
		SDL_RenderCopy(AppRenderer, SIDESCROLLICON_Texture, NULL, &SideScrollTemplateIcon);
		SDL_RenderCopy(AppRenderer, SIDESCROLLTEXT_Texture, NULL, &SideScrollTextRect);
		SDL_QueryTexture(PROJECTTEMPLATETEXT_Texture, NULL, NULL, &nptttw, &npttth);
		SDL_Rect TemplateTitleTextRect{ window_width * 0.3 - nptttw / 2, (window_height * 0.37), nptttw, npttth };
		SDL_RenderCopy(AppRenderer, PROJECTTEMPLATETEXT_Texture, NULL, &TemplateTitleTextRect);
		SDL_RenderPresent(AppRenderer);
	}
	if (window_state == SPICY_MAIN_MENU) {
		SDL_RenderClear(AppRenderer);
		SDL_SetRenderDrawColor(AppRenderer, 255, 255, 255, 255);
		SDL_Rect Background{ 0,0, window_width, window_height };
		SDL_SetRenderDrawColor(AppRenderer, 0x2D, 0x2D, 0x30, 255);
		SDL_RenderFillRect(AppRenderer, &Background);
		SDL_Rect Foreground{ 0, window_height * 0.2, window_width, window_height * 0.8 };
		SDL_SetRenderDrawColor(AppRenderer, 40, 40, 43, 255);
		SDL_RenderFillRect(AppRenderer, &Foreground);
		SDL_Rect AppBar{ 0, 0, window_width, window_height * 0.02 };
		SDL_SetRenderDrawColor(AppRenderer, 0x5A, 0x5A, 0x5A, 255);
		SDL_RenderFillRect(AppRenderer, &AppBar);
		SDL_Rect NewProjectRect{ window_width * 0.25, window_height * 0.05, window_width * 0.2, window_height * 0.1 };
		SDL_SetRenderDrawColor(AppRenderer, 0x2D, 0x1D, 0x2D, 255);
		SDL_Rect NewProjectRectText{ window_width * 0.26, window_height * 0.078, window_width * 0.18, window_height * 0.045 };
		SDL_RenderFillRect(AppRenderer, &NewProjectRect);
		SDL_RenderCopy(AppRenderer, CP1_Texture, NULL, &NewProjectRectText);
		SDL_Rect SelectProjectRect{ window_width * 0.03, window_height * 0.05, window_width * 0.2, window_height * 0.1 };
		SDL_Rect SelectProjectRectText{ window_width * 0.04, window_height * 0.078, window_width * 0.13, window_height * 0.043 };
		SDL_SetRenderDrawColor(AppRenderer, 0x2D, 0x1D, 0x2D, 255);
		SDL_RenderFillRect(AppRenderer, &SelectProjectRect);
		SDL_RenderCopy(AppRenderer, SP1_Texture, NULL, &SelectProjectRectText);
		SDL_Rect QuitButtonRect{ window_width * 0.96, 0, window_width * 0.04, window_height * 0.02 };
		if (mouse.x > window_width * 0.96) {
			if (mouse.y < window_height * 0.02) {
				SDL_SetRenderDrawBlendMode(AppRenderer, SDL_BLENDMODE_ADD);
				SDL_RenderCopy(AppRenderer, QuitButtonTexture, NULL, &QuitButtonRect);
				SDL_SetRenderDrawColor(AppRenderer, 255, 50, 50, 150);
				SDL_RenderFillRect(AppRenderer, &QuitButtonRect);
				SDL_SetRenderDrawBlendMode(AppRenderer, SDL_BLENDMODE_NONE);
			}
			else {
				SDL_RenderCopy(AppRenderer, QuitButtonTexture, NULL, &QuitButtonRect);
			}
		}
		else {
			SDL_RenderCopy(AppRenderer, QuitButtonTexture, NULL, &QuitButtonRect);
		}
		SDL_RenderPresent(AppRenderer);
	}
	if (window_state == SPICY_CANVAS) {
		if (editor.moving_camera) {
			if (ctrl_key_pressed) {
				editor.editor_camera.x += mouse.dx;
				editor.editor_camera.y += mouse.dy;
			}
		}
		SDL_SetRenderDrawColor(AppRenderer, 45, 45, 48, 255);
		SDL_RenderClear(AppRenderer);
		SDL_Rect Canvas{ window_width * 0.15, window_height * 0.2, window_width * 0.85, window_height * 0.8 };
		SDL_SetRenderDrawColor(AppRenderer, 25, 25, 25, 255);
		SDL_RenderFillRect(AppRenderer, &Canvas);
		if (!editor.text_editor_section_showing) {
			if (Viewer.grid_lines) {
				SDL_Rect Y_Axis{ editor.editor_camera.x - Viewer.primary_axis_size / 2 + window_width * 0.15, 0, Viewer.primary_axis_size,  window_height };
				SDL_SetRenderDrawColor(AppRenderer, 255, 0, 0, 255);
				SDL_RenderFillRect(AppRenderer, &Y_Axis);
				SDL_Rect X_Axis{ 0, editor.editor_camera.y - Viewer.primary_axis_size / 2 + window_height * 0.2, window_width, Viewer.primary_axis_size };
				SDL_RenderFillRect(AppRenderer, &X_Axis);
				SDL_Rect minor_y_right_axis{ (-1 * floor(editor.editor_camera.x / window_width) * window_width) + editor.editor_camera.x + window_width * 0.15 + window_width, 0, Viewer.primary_axis_size / 2, window_height };
				SDL_Rect minor_y_left_axis{ (-1 * floor(editor.editor_camera.x / window_width) * window_width) + editor.editor_camera.x + window_width * 0.15, 0, Viewer.primary_axis_size / 2, window_height };
				SDL_SetRenderDrawColor(AppRenderer, 140, 140, 140, 255);
				SDL_RenderFillRect(AppRenderer, &minor_y_right_axis);
				SDL_RenderFillRect(AppRenderer, &minor_y_left_axis);
				SDL_Rect minor_x_right_axis{ 0, (-1 * floor(editor.editor_camera.y / window_height) * window_height) + editor.editor_camera.y + window_height * 0.2 + window_height, window_width, Viewer.primary_axis_size / 2 };
				SDL_Rect minor_x_left_axis{ 0, (-1 * floor(editor.editor_camera.y / window_height) * window_height) + editor.editor_camera.y + window_height * 0.2, window_width, Viewer.primary_axis_size / 2 };
				SDL_RenderFillRect(AppRenderer, &minor_x_left_axis);
				SDL_RenderFillRect(AppRenderer, &minor_x_right_axis);
			}
			if (!editor.deleting_object) {
				for (auto layer : game.layers) {
					layer.Draw(AppRenderer, &game, &editor.editor_camera, this->Viewer);
				}
			}
			else {
				if (Viewer.rectangles) {
					for (int rectangle = 0; rectangle < game.cp_rect_data.size(); rectangle++) {
						SDL_Rect Rect{ game.cp_rect_data[rectangle].x + window_width * 0.15 + editor.editor_camera.x, game.cp_rect_data[rectangle].y + window_height * 0.2 + editor.editor_camera.y, game.cp_rect_data[rectangle].w, game.cp_rect_data[rectangle].h };

						if (editor.deleting_object && !IsMouseInRect(mouse, Rect)) {
							SDL_SetRenderDrawColor(AppRenderer, game.cp_rect_data[rectangle].r, game.cp_rect_data[rectangle].g, game.cp_rect_data[rectangle].b, 128);
							SDL_SetRenderDrawBlendMode(AppRenderer, SDL_BLENDMODE_BLEND);
						}
						else {
							SDL_SetRenderDrawBlendMode(AppRenderer, SDL_BLENDMODE_NONE);
							SDL_SetRenderDrawColor(AppRenderer, game.cp_rect_data[rectangle].r, game.cp_rect_data[rectangle].g, game.cp_rect_data[rectangle].b, 255);
						}
						SDL_RenderFillRect(AppRenderer, &Rect);
					}
				}
				if (Viewer.textures) {
					for (int texture = 0; texture < game.cp_texture_data.size(); texture++) {
						SDL_SetTextureBlendMode(*game.cp_texture_data[texture].texture.content, SDL_BLENDMODE_BLEND);
						SDL_Rect TextureRect{ game.cp_texture_data[texture].x + window_width * 0.15 + editor.editor_camera.x, game.cp_texture_data[texture].y + window_height * 0.2 + editor.editor_camera.y, game.cp_texture_data[texture].w, game.cp_texture_data[texture].h };
						if (editor.deleting_object && !IsMouseInRect(mouse, TextureRect)) {
							SDL_SetTextureAlphaMod(*game.cp_texture_data[texture].texture.content, 128);
							SDL_RenderCopyEx(AppRenderer, *game.cp_texture_data[texture].texture.content, NULL, &TextureRect, 0, NULL, SDL_FLIP_NONE);
						}
						else {
							SDL_RenderCopyEx(AppRenderer, *game.cp_texture_data[texture].texture.content, NULL, &TextureRect, game.cp_texture_data[texture].rotation_angle, NULL, SDL_FLIP_NONE);
						}
						SDL_SetTextureBlendMode(*game.cp_texture_data[texture].texture.content, SDL_BLENDMODE_NONE);
					}
				}
			}
			if (Viewer.rect_walls) {
				for (int rect_wall = 0; rect_wall < game.cp_rect_wall_data.size(); rect_wall++) {
					SDL_Rect Wall{ game.cp_rect_wall_data[rect_wall].x + window_width * 0.15 + editor.editor_camera.x, game.cp_rect_wall_data[rect_wall].y + window_height * 0.2 + editor.editor_camera.y, game.cp_rect_wall_data[rect_wall].w, game.cp_rect_wall_data[rect_wall].h };
					if (editor.deleting_object && !IsMouseInRect(mouse, Wall)) {

					}
					else {

					}
					SDL_SetRenderDrawBlendMode(AppRenderer, SDL_BLENDMODE_ADD);
					SDL_SetRenderDrawColor(AppRenderer, 255, 128, 0, 128);
					SDL_RenderFillRect(AppRenderer, &Wall);
					SDL_SetRenderDrawBlendMode(AppRenderer, SDL_BLENDMODE_NONE);

				}
			}
			if (editor.creating_rectangle) {
				if (editor.creation_points.size() == 1) {
					SDL_SetRenderDrawBlendMode(AppRenderer, SDL_BLENDMODE_BLEND);
					SDL_Rect Tracer{ editor.creation_points[0].x + window_width * 0.15 + editor.editor_camera.x, editor.creation_points[0].y + window_height * 0.2 + editor.editor_camera.y, mouse.x - editor.creation_points[0].x - window_width * 0.15 - editor.editor_camera.x, mouse.y - editor.creation_points[0].y - window_height * 0.2 - editor.editor_camera.y };
					SDL_SetRenderDrawColor(AppRenderer, editor.color[0], editor.color[1], editor.color[2], 128);
					SDL_RenderFillRect(AppRenderer, &Tracer);
					SDL_SetRenderDrawBlendMode(AppRenderer, SDL_BLENDMODE_NONE);
				}
			}
			if (editor.creating_image) {
				if (editor.creation_points.size() == 1) {
					SDL_SetRenderDrawBlendMode(AppRenderer, SDL_BLENDMODE_BLEND);
					SDL_Rect Tracer{ editor.creation_points[0].x + window_width * 0.15 + editor.editor_camera.x, editor.creation_points[0].y + window_height * 0.2 + editor.editor_camera.y, mouse.x - editor.creation_points[0].x - window_width * 0.15 - editor.editor_camera.x, mouse.y - editor.creation_points[0].y - window_height * 0.2 - editor.editor_camera.y };
					SDL_SetRenderDrawColor(AppRenderer, editor.color[0], editor.color[1], editor.color[2], 128);
					SDL_RenderFillRect(AppRenderer, &Tracer);
					SDL_SetRenderDrawBlendMode(AppRenderer, SDL_BLENDMODE_NONE);
				}
				if (editor.creation_points.size() == 2) {
					SDL_Rect Tracer{ editor.creation_points[0].x + window_width * 0.15 + editor.editor_camera.x, editor.creation_points[0].y + window_height * 0.2 + editor.editor_camera.y, editor.creation_points[1].x - editor.creation_points[0].x, editor.creation_points[1].y - editor.creation_points[0].y };
					SDL_RenderCopyEx(AppRenderer, *editor.current_texture.content, NULL, &Tracer, editor.creation_points[1].x - (mouse.x - window_width * 0.15 - editor.editor_camera.x), NULL, SDL_FLIP_NONE);
				}
			}

			if (editor.creating_rect_wall) {
				if (editor.creation_points.size() == 1) {
					SDL_SetRenderDrawBlendMode(AppRenderer, SDL_BLENDMODE_BLEND);
					SDL_Rect Tracer{ editor.creation_points[0].x + window_width * 0.15 + editor.editor_camera.x, editor.creation_points[0].y + window_height * 0.2 + editor.editor_camera.y, mouse.x - editor.creation_points[0].x - window_width * 0.15 - editor.editor_camera.x, mouse.y - editor.creation_points[0].y - window_height * 0.2 - editor.editor_camera.y };
					SDL_SetRenderDrawColor(AppRenderer, 255, 128, 0, 128);
					SDL_RenderFillRect(AppRenderer, &Tracer);
					SDL_SetRenderDrawBlendMode(AppRenderer, SDL_BLENDMODE_NONE);
				}
			}
		}
		else if (editor.text_editor_section_showing) {
			SDL_Rect margin_line{ window_width * 0.195 + editor.editor_camera.x, window_height * 0.15, 3, window_height * 0.85 };
			SDL_SetRenderDrawColor(AppRenderer, 255, 255, 255, 255);
			SDL_RenderFillRect(AppRenderer, &margin_line);
			editor.line_number = 0;
			for (int x = 0; x < editor.lines.size(); x++) {
				editor.drawText(editor.lines[x], AppRenderer, WHITE - 1);
			}
			SDL_Rect character{ window_width * 0.2 + (editor.cursor_spacing + 1) * 15 + editor.editor_camera.x, window_width * 0.2 + editor.cursor_line_number * 24 + editor.editor_camera.y, 3, 24 };
			if (editor.editing_editor_text) {
				SDL_SetRenderDrawColor(AppRenderer, 255, 255, 255, 255);
			}
			else {
				SDL_SetRenderDrawColor(AppRenderer, 150, 150, 150, 255);
			}
			SDL_RenderFillRect(AppRenderer, &character);
			if (editor.show_error_section) {
				SDL_Rect background{ window_width * 0.15, window_height * 0.8, window_width * 0.85, window_height * 0.2 };
				SDL_SetRenderDrawColor(AppRenderer, 45, 45, 48, 255);
				SDL_RenderFillRect(AppRenderer, &background);
				editor.line_number = 0;
				editor.RenderErrors(AppRenderer);
			}
		}
		SDL_Rect SwapDevModeRect{ window_width * 0.95, window_height * 0.2, window_width * 0.05, window_width * 0.05};
		SDL_RenderCopy(AppRenderer, SWAPDEVMODEICON_Texture, NULL, &SwapDevModeRect);
		SDL_SetRenderDrawColor(AppRenderer, 45, 45, 48, 255);
		SDL_Rect Background{ 0, 0, window_width, window_height * 0.2 };
		SDL_RenderFillRect(AppRenderer, &Background);
		SDL_Rect SideBar{ 0, window_height * 0.2, window_width * 0.15, window_height * 0.8 };
		SDL_SetRenderDrawColor(AppRenderer, 40, 40, 43, 255);
		SDL_RenderFillRect(AppRenderer, &SideBar);
		SDL_Rect DropDownBar{ 0, window_height * 0.2, window_width * 0.01, window_height * 0.8 };
		editor.DropDownBar.set_cords(DropDownBar);
		SDL_SetRenderDrawColor(AppRenderer, 100, 100, 100, 255);
		SDL_RenderFillRect(AppRenderer, &DropDownBar);
		editor.DropDownBar.size = 100;
		SDL_SetRenderDrawColor(AppRenderer, 75, 75, 75, 255);
		SDL_Rect DropDownBarFillMarker{ 0, window_height - editor.DropDownBar.getFill(), window_width * 0.01, editor.DropDownBar.size * -1 };
		if (editor.DropDownBar.getFill() <= 0) {
			DropDownBarFillMarker.y = window_height;
		}
		SDL_RenderFillRect(AppRenderer, &DropDownBarFillMarker);
		if (editor.moving_DropDownBar) {
			if (editor.DropDownBar.getFill() >= 0) {
				editor.DropDownBar.UpdateFill(mouse, true);
			}
			else {
				editor.DropDownBar.setFill(0);
			}
		}
		int drop_down_buffer = 0;
		int buffer = 0;
		int fsttw;
		int fstth;
		SDL_QueryTexture(FILEVIEWS_Texture, NULL, NULL, &fsttw, &fstth);
		SDL_Rect FileSectionRect{ window_width * 0.032, window_height * 0.22 - editor.DropDownBar.getFill(), fsttw, fstth };
		SDL_Rect _FileSectionRect{ window_width * 0.005, window_height * 0.218 - editor.DropDownBar.getFill(), window_width * 0.145, window_height * 0.015 };
		if (IsMouseInRect(mouse, _FileSectionRect)) {
			SDL_SetRenderDrawColor(AppRenderer, 100, 100, 100, 255);
			SDL_RenderFillRect(AppRenderer, &_FileSectionRect);
		}
		SDL_RenderCopy(AppRenderer, FILEVIEWS_Texture, NULL, &FileSectionRect);
		if (!editor.collapse_file_section) {
			for (int x = 0; x < editor.view_file_textures.size(); x++) {
				SDL_Rect bullet_point{ window_width * 0.035, window_height * 0.24 + (window_height * x / 50) - editor.DropDownBar.getFill(), window_width * 0.003, window_width * 0.003 };
				SDL_SetRenderDrawColor(AppRenderer, 255, 0, 0, 255);
				SDL_RenderFillRect(AppRenderer, &bullet_point);
				int currentw;
				int currenth;
				SDL_QueryTexture(editor.view_file_textures[x], NULL, NULL, &currentw, &currenth);
				SDL_Rect TextGoesHere{ window_width * 0.044, window_height * 0.235 + (window_height * x / 50) - editor.DropDownBar.getFill(), currentw, currenth };
				if (IsMouseInRect(mouse, TextGoesHere)) {
					SDL_SetRenderDrawColor(AppRenderer, 100, 100, 100, 255);
					SDL_RenderFillRect(AppRenderer, &TextGoesHere);
				}
				if (editor.view_file_index == x) {
					SDL_SetRenderDrawColor(AppRenderer, 100, 150, 100, 255);
					SDL_RenderFillRect(AppRenderer, &TextGoesHere);
				}
				SDL_RenderCopy(AppRenderer, editor.view_file_textures[x], NULL, &TextGoesHere);
				buffer += (window_height * x / 100);
			}
		}
		buffer += (window_height * 0.05);
		int isttw;
		int istth;
		SDL_QueryTexture(IMAGEVIEWS_Texture, NULL, NULL, &isttw, &istth);
		SDL_Rect ImageSectionRect{ window_width * 0.032, window_height * 0.22 + buffer - editor.DropDownBar.getFill(), isttw, istth };
		SDL_Rect _ImageSectionRect{ window_width * 0.005, window_height * 0.218 + buffer - editor.DropDownBar.getFill(), window_width * 0.145, window_height * 0.015 };
		if (IsMouseInRect(mouse, _ImageSectionRect)) {
			SDL_SetRenderDrawColor(AppRenderer, 100, 100, 100, 255);
			SDL_RenderFillRect(AppRenderer, &_ImageSectionRect);
		}
		SDL_RenderCopy(AppRenderer, IMAGEVIEWS_Texture, NULL, &ImageSectionRect);
		if (!editor.collapse_image_section) {
			for (int x = 0; x < editor.view_image_textures.size(); x++) {
				SDL_Rect bullet_point{ window_width * 0.035, window_height * 0.24 + (window_height * x / 50) + buffer - editor.DropDownBar.getFill(), window_width * 0.003, window_width * 0.003 };
				SDL_SetRenderDrawColor(AppRenderer, 255, 0, 0, 255);
				SDL_RenderFillRect(AppRenderer, &bullet_point);
				int currentw;
				int currenth;
				SDL_QueryTexture(editor.view_image_textures[x], NULL, NULL, &currentw, &currenth);
				SDL_Rect TextGoesHere{ window_width * 0.044, window_height * 0.235 + (window_height * x / 50) + buffer - editor.DropDownBar.getFill(), currentw, currenth };
				if (IsMouseInRect(mouse, TextGoesHere)) {
					SDL_SetRenderDrawColor(AppRenderer, 100, 100, 100, 255);
					SDL_RenderFillRect(AppRenderer, &TextGoesHere);
				}
				if (editor.view_image_index == x) {
					SDL_SetRenderDrawColor(AppRenderer, 100, 150, 100, 255);
					SDL_RenderFillRect(AppRenderer, &TextGoesHere);
				}
				SDL_RenderCopy(AppRenderer, editor.view_image_textures[x], NULL, &TextGoesHere);
			}
		}
		int ivttw;
		int ivtth;
		SDL_QueryTexture(IMAGEVIEW_Texture, NULL, NULL, &ivttw, &ivtth);
		SDL_Rect ImageViewTextRect{ window_width * 0.06, window_height * 0.2, ivttw, ivtth };
		SDL_RenderCopy(AppRenderer, IMAGEVIEW_Texture, NULL, &ImageViewTextRect);
		SDL_Rect Patch{ 0, window_height * 0.02, window_width * 0.2, window_height * 0.18 };
		SDL_SetRenderDrawColor(AppRenderer, 0x2D, 0x2D, 0x30, 255);
		SDL_RenderFillRect(AppRenderer, &Patch);
		SDL_Rect AppBar{ 0, 0, window_width, window_height * 0.04 };
		SDL_SetRenderDrawColor(AppRenderer, 0x5A, 0x5A, 0x5A, 255);
		SDL_RenderFillRect(AppRenderer, &AppBar);
		for (int x = 0; x < ribbons.size(); x++) {
			ribbons[x]->draw(AppRenderer, x);
			if ((ribbons[x])->active) {
				if (x == ribbon_home) { // HOME
					SDL_SetRenderDrawColor(AppRenderer, 100, 100, 100, 255);
					SDL_Rect HomeLabelRect{ window_width * 0.05, window_height * 0.07, window_width * 0.0425, window_height * 0.07 };
					int mmttw;
					int mmtth;
					SDL_QueryTexture(HOMETEXT_Texture, NULL, NULL, &mmttw, &mmtth);
					SDL_Rect HomeLabelTextRect{ window_width * 0.052, window_height * 0.12, mmttw, mmtth};
					
					if (IsMouseInRect(mouse, HomeLabelRect)) {
						SDL_RenderFillRect(AppRenderer, &HomeLabelRect);
					}
					SDL_Rect HomeLabelRectIcon{ window_width * 0.05, window_height * 0.07, window_width * 0.0425, window_height * 0.06 };
					SDL_RenderCopy(AppRenderer, HOMEICON_Texture, NULL, &HomeLabelRectIcon);
					SDL_RenderCopy(AppRenderer, HOMETEXT_Texture, NULL, &HomeLabelTextRect);
					SDL_Rect DocuLabelRect{ window_width * 0.1125, window_height * 0.07, window_width * 0.0425 * 1.25, window_height * 0.07 }; // WIDTH: + 0.02
					int dcttw;
					int dctth;
					SDL_QueryTexture(DOCUTEXT_Texture, NULL, NULL, &dcttw, &dctth);
					if (IsMouseInRect(mouse, DocuLabelRect)) {
						SDL_RenderFillRect(AppRenderer, &DocuLabelRect);
					}
					SDL_Rect DocuLabelTextRect{ window_width * 0.1135, window_height * 0.12, dcttw, dctth};
					SDL_Rect DocuLabelIcon{ window_width * 0.1125, window_height * 0.07, window_width * 0.0425 * 1.25, window_height * 0.06 };
					SDL_RenderCopy(AppRenderer, DOCUTEXT_Texture, NULL, &DocuLabelTextRect);
					SDL_RenderCopy(AppRenderer, DOCUICON_Texture, NULL, &DocuLabelIcon);
				}
				else if (x == ribbon_graphic) {
					SDL_SetRenderDrawColor(AppRenderer, 100, 100, 100, 255);
					SDL_Rect RectangleLabelRect{ window_width * 0.05, window_height * 0.07, window_width * 0.0425, window_height * 0.07 };
					int mmttw;
					int mmtth;
					SDL_QueryTexture(RECTANGLETEXT_Texture, NULL, NULL, &mmttw, &mmtth);
					SDL_Rect RectangleLabelTextRect{ window_width * 0.054, window_height * 0.12, mmttw, mmtth };

					if (IsMouseInRect(mouse, RectangleLabelRect)) {
		 				SDL_RenderFillRect(AppRenderer, &RectangleLabelRect);
					}
					if (editor.creating_rectangle) {
						SDL_SetRenderDrawColor(AppRenderer, 100, 100, 140, 255);
						SDL_RenderFillRect(AppRenderer, &RectangleLabelRect);
					}
					SDL_Rect RectangleLabelRectIcon{ window_width * 0.05, window_height * 0.07, window_width * 0.0425, window_height * 0.06 };
					SDL_RenderCopy(AppRenderer, RECTANGLEICON_Texture, NULL, &RectangleLabelRectIcon);
					SDL_RenderCopy(AppRenderer, RECTANGLETEXT_Texture, NULL, &RectangleLabelTextRect);

					SDL_SetRenderDrawColor(AppRenderer, 100, 100, 100, 255);
					SDL_Rect ImageLabelRect{ window_width * 0.12, window_height * 0.07, window_width * 0.0425, window_height * 0.07 };
					int iittw;
					int iitth;
					SDL_QueryTexture(IMAGETEXT_Texture, NULL, NULL, &iittw, &iitth);
					SDL_Rect ImageLabelTextRect{ window_width * 0.13, window_height * 0.12, iittw, iitth };

					if (IsMouseInRect(mouse, ImageLabelRect)) {
						SDL_RenderFillRect(AppRenderer, &ImageLabelRect);
					}
					if (editor.creating_image) {
						SDL_SetRenderDrawColor(AppRenderer, 100, 100, 140, 255);
						SDL_RenderFillRect(AppRenderer, &ImageLabelRect);
					}
					SDL_Rect ImageLabelRectIcon{ window_width * 0.12, window_height * 0.07, window_width * 0.0425, window_height * 0.06 };
					SDL_RenderCopy(AppRenderer,IMAGEICON_Texture, NULL, &ImageLabelRectIcon);
					SDL_RenderCopy(AppRenderer, IMAGETEXT_Texture, NULL, &ImageLabelTextRect);

					SDL_SetRenderDrawColor(AppRenderer, 100, 100, 100, 255);
					SDL_Rect PrRectangleLabelRect{ window_width * 0.19, window_height * 0.07, window_width * 0.0425, window_height * 0.07 };
					SDL_QueryTexture(SPECRECTANGLETEXT_Texture, NULL, NULL, &mmttw, &mmtth);
					SDL_Rect PrRectangleLabelTextRect{ window_width * 0.194, window_height * 0.12, mmttw, mmtth };

					if (IsMouseInRect(mouse, PrRectangleLabelRect)) {
						SDL_RenderFillRect(AppRenderer, &PrRectangleLabelRect);
					}
					SDL_Rect PrRectangleLabelRectIcon{ window_width * 0.19, window_height * 0.07, window_width * 0.0425, window_height * 0.06 };
					SDL_RenderCopy(AppRenderer, RECTANGLEICON_Texture, NULL, &PrRectangleLabelRectIcon);
					SDL_RenderCopy(AppRenderer, SPECRECTANGLETEXT_Texture, NULL, &PrRectangleLabelTextRect);

					SDL_SetRenderDrawColor(AppRenderer, 100, 100, 100, 255);
					SDL_Rect PrTextureLabelRect{ window_width * 0.26, window_height * 0.07, window_width * 0.0425, window_height * 0.07 };
					SDL_QueryTexture(SPECRECTANGLETEXT_Texture, NULL, NULL, &mmttw, &mmtth);
					SDL_Rect PrTextureLabelTextRect{ window_width * 0.264, window_height * 0.12, mmttw, mmtth };

					if (IsMouseInRect(mouse, PrTextureLabelRect)) {
						SDL_RenderFillRect(AppRenderer, &PrTextureLabelRect);
					}
					SDL_Rect PrTextureLabelRectIcon{ window_width * 0.26, window_height * 0.07, window_width * 0.0425, window_height * 0.06 };
					SDL_RenderCopy(AppRenderer, IMAGEICON_Texture, NULL, &PrTextureLabelRectIcon);
					SDL_RenderCopy(AppRenderer, SPECIMAGETEXT_Texture, NULL, &PrTextureLabelTextRect);

				}
				else if (x == ribbon_interactive) {
					SDL_SetRenderDrawColor(AppRenderer, 100, 100, 100, 255);
					SDL_Rect RectangleWallLabelRect{ window_width * 0.05, window_height * 0.07, window_width * 0.0425, window_height * 0.07 };
					int mmttw;
					int mmtth;
					SDL_QueryTexture(RECTWALLTEXT_Texture, NULL, NULL, &mmttw, &mmtth);
					SDL_Rect RectangleWallLabelTextRect{ window_width * 0.064, window_height * 0.12, mmttw, mmtth };

					if (IsMouseInRect(mouse, RectangleWallLabelRect) || editor.creating_rect_wall) {
						SDL_RenderFillRect(AppRenderer, &RectangleWallLabelRect);
					}
					SDL_Rect RectangleWallLabelRectIcon{ window_width * 0.05, window_height * 0.07, window_width * 0.0425, window_height * 0.06 };
					SDL_RenderCopy(AppRenderer, RECTWALLICON_Texture, NULL, &RectangleWallLabelRectIcon);
					SDL_RenderCopy(AppRenderer, RECTWALLTEXT_Texture, NULL, &RectangleWallLabelTextRect);
				}
				else if (x == ribbon_layer) {
					SDL_SetRenderDrawColor(AppRenderer, 100, 100, 100, 255);
					SDL_Rect AddLayerLabelRect{ window_width * 0.05, window_height * 0.07, window_width * 0.0425, window_height * 0.07 };
					if (IsMouseInRect(mouse, AddLayerLabelRect)) {
						SDL_RenderFillRect(AppRenderer, &AddLayerLabelRect);
					}
					SDL_Rect AddLayerIconLabel{ window_width * 0.05, window_height * 0.06, window_width * 0.0425, window_height * 0.07 };
					SDL_RenderCopy(AppRenderer, ADDLAYERICON_Texture, NULL, &AddLayerIconLabel);
					int alttw;
					int altth;
					SDL_QueryTexture(ADDLAYER_Texture, NULL, NULL, &alttw, &altth);
					SDL_Rect AddLayerTextRect{ window_width * 0.054, window_height * 0.12, alttw, altth };
					SDL_RenderCopy(AppRenderer, ADDLAYER_Texture, NULL, &AddLayerTextRect);
					int rlttw;
					int rltth;
					SDL_QueryTexture(REMOVELAYER_Texture, NULL, NULL, &rlttw, &rltth);
					SDL_Rect RemoveLayerLabelRect{ window_width * 0.13, window_height * 0.07, window_width * 0.0425 + rlttw / 3, window_height * 0.07 };
					if (IsMouseInRect(mouse, RemoveLayerLabelRect)) {
						SDL_RenderFillRect(AppRenderer, &RemoveLayerLabelRect);
					}
					SDL_Rect RemoveLayerTextRect{ window_width * 0.134, window_height * 0.12, rlttw, rltth };
					SDL_Rect RemoveLayerIconLabel{ window_width * 0.13 + ((window_width * 0.0425 + rlttw / 3) / 4) , AddLayerIconLabel.y, (window_width * 0.0425 + rlttw / 3) / 2, AddLayerIconLabel.h };
					SDL_RenderCopy(AppRenderer, REMOVELAYER_Texture, NULL, &RemoveLayerTextRect);
					SDL_RenderCopy(AppRenderer, REMOVELAYERICON_Texture, NULL, &RemoveLayerIconLabel);
					SDL_Rect LayerUpButton{ window_width * 0.18 + window_width * 0.0425 + rlttw / 3, window_height * 0.07, window_width * 0.0425, window_height * 0.015 };
					SDL_RenderCopy(AppRenderer, UPARROW_Texture, NULL, &LayerUpButton);

				}
				else if (x == ribbon_edit) {

				}
			}
		}
		SDL_Rect Icon{ 0, 0, window_height * 0.02, window_height * 0.02 };
		SDL_RenderCopy(AppRenderer, WINICON_Texture, NULL, &Icon);
		SDL_Rect StartButton{ window_width * 0.93, window_height * 0.17, window_width * 0.1, window_height * 0.03 };
		if (IsMouseInRect(mouse, StartButton) && !editor.file_section_showing)
			SDL_SetRenderDrawColor(AppRenderer, 45, 200, 45, 255);
		else
			SDL_SetRenderDrawColor(AppRenderer, 55, 55, 60, 255);
		SDL_RenderFillRect(AppRenderer, &StartButton);
		int sbttw;
		int sbtth;
		SDL_QueryTexture(START_Texture, NULL, NULL, &sbttw, &sbtth);
		SDL_Rect StartButtonTextRect{ window_width * 0.94, window_height * 0.172, sbttw, sbtth };
		SDL_RenderCopy(AppRenderer, START_Texture, NULL, &StartButtonTextRect);
		{
			ConvertButton.x = window_width * 0.93;
			ConvertButton.y = window_height * 0.14;
			ConvertButton.width = window_width * 0.1;
			ConvertButton.height = window_height * 0.03;
			ConvertButton.Draw(&mouse, AppRenderer);
		}
		SDL_Rect CameraRelationButton{ 0, (window_height * 0.168) + 3, window_width * 0.15, window_height * 0.03 };
		if (IsMouseInRect(mouse, CameraRelationButton)) {
			SDL_SetRenderDrawColor(AppRenderer, 100, 100, 100, 255);
			SDL_RenderFillRect(AppRenderer, &CameraRelationButton);
		}
		int crttw;
		int crtth;
		SDL_QueryTexture(DYNAMICSTATICSTATE_Texture, NULL, NULL, &crttw, &crtth);
		SDL_Rect CameraRelationTextRect{ window_width * 0.0124, window_height * 0.172, crttw, crtth };
		SDL_RenderCopy(AppRenderer, DYNAMICSTATICSTATE_Texture, NULL, &CameraRelationTextRect);
		SDL_Rect FileButton{ window_width * 0.015, window_height * 0.004, window_width * 0.02, window_height * 0.014 };
		int ctttw;
		int cttth;
		SDL_QueryTexture(CTEXTUREUI_Texture, NULL, NULL, &ctttw, &cttth);
		SDL_Rect CurrentTextureTextRect{ window_width * 0.16, window_height * 0.172, ctttw, cttth };
		SDL_RenderCopy(AppRenderer, CTEXTUREUI_Texture, NULL, &CurrentTextureTextRect);
		if (IsMouseInRect(mouse, FileButton))
			SDL_SetRenderDrawColor(AppRenderer, 110, 110, 110, 255);
		else
			SDL_SetRenderDrawColor(AppRenderer, 90, 90, 90, 255);
		int clttw;
		int cltth;
		SDL_QueryTexture(CURRENTLAYER_Texture, NULL, NULL, &clttw, &cltth);
		SDL_Rect CurrentLayerTextRect{ window_width * 0.3, window_height * 0.172, clttw, cltth };
		SDL_RenderCopy(AppRenderer, CURRENTLAYER_Texture, NULL, &CurrentLayerTextRect);
		SDL_Rect QuitButtonRect{ window_width * 0.96, 0, window_width * 0.04, window_height * 0.02 };
		SDL_Rect AboutIconRect{ window_width * 0.95, 0, window_width * 0.01, window_height * 0.02 };
		if (IsMouseInRect(mouse, AboutIconRect)) {
			SDL_SetRenderDrawColor(AppRenderer, 110, 110, 110, 255);
			SDL_RenderFillRect(AppRenderer, &AboutIconRect);
		}
		SDL_RenderCopy(AppRenderer, ABOUTICON_Texture, NULL, &AboutIconRect);
		if (mouse.x > window_width * 0.96) {
			if (mouse.y < window_height * 0.02) {
				SDL_SetRenderDrawBlendMode(AppRenderer, SDL_BLENDMODE_ADD);
				SDL_RenderCopy(AppRenderer, QuitButtonTexture, NULL, &QuitButtonRect);
				SDL_SetRenderDrawColor(AppRenderer, 255, 50, 50, 150);
				SDL_RenderFillRect(AppRenderer, &QuitButtonRect);
				SDL_SetRenderDrawBlendMode(AppRenderer, SDL_BLENDMODE_NONE);
			}
			else {
				SDL_RenderCopy(AppRenderer, QuitButtonTexture, NULL, &QuitButtonRect);
			}
		}
		else {
			SDL_RenderCopy(AppRenderer, QuitButtonTexture, NULL, &QuitButtonRect);
		}

		SDL_RenderPresent(AppRenderer);
	}
	if (window_state == SPICY_GAME) {
		SDL_SetRenderDrawColor(AppRenderer, 0, 0, 0, 0);
		SDL_RenderClear(AppRenderer);
		std::vector<GRWall*> dynamic_walls = {};
		std::vector<GRWall*> static_walls = {};
		for (int x = 0; x < editor.preview_game.cp_rect_wall_data.size(); x++) {
			if (editor.preview_game.cp_rect_wall_data[x].dynamic_static == SPICY_STATIC) {
				static_walls.push_back(&editor.preview_game.cp_rect_wall_data[x]);
			}
			else {
				dynamic_walls.push_back(&editor.preview_game.cp_rect_wall_data[x]);
			}
		}
		vec2 camera_force = { 0, 0 };
		static bool can_jump = true;
		if (editor.preview_game.game_camera.up && editor.preview_game.game_camera.down) {
			if (!editor.preview_game.enable_gravity) {
				editor.preview_game.game_camera.y_direction = Direction::None;
			}
			if (editor.preview_game.enable_gravity) {
				editor.preview_game.game_camera.y_direction = Direction::Down;
			}
		}
		else if (editor.preview_game.game_camera.up) {
			if (!editor.preview_game.enable_gravity) {
				editor.preview_game.game_camera.y_direction = Direction::Up;
			}
			else {
				if (editor.preview_game.game_camera.y_direction != Direction::Down && can_jump) {
					editor.preview_game.game_camera.in_air = true;
					editor.preview_game.game_camera.y_direction = Direction::Up;
					can_jump = false;
				}
			}
		}
		else if (editor.preview_game.game_camera.down) {
			if (!editor.preview_game.enable_gravity) {
				editor.preview_game.game_camera.y_direction = Direction::Down;
			}
		}
		else {
			if (!editor.preview_game.enable_gravity) {
				editor.preview_game.game_camera.y_direction = Direction::None;
			}
		}
		
		if (editor.preview_game.game_camera.left && editor.preview_game.game_camera.right) {
			editor.preview_game.game_camera.x_direction = Direction::None;
		}
		else if (editor.preview_game.game_camera.left) {
			editor.preview_game.game_camera.x_direction = Direction::Left;
		}
		else if (editor.preview_game.game_camera.right) {
			editor.preview_game.game_camera.x_direction = Direction::Right;
		}
		else {
			editor.preview_game.game_camera.x_direction = Direction::None;
		}
		if (editor.preview_game.game_camera.x_direction == Direction::Left) {
			camera_force.x -= editor.preview_game.camera_speed * clock.deltaTime;
		}
		if (editor.preview_game.game_camera.x_direction == Direction::Right) {
			camera_force.x += editor.preview_game.camera_speed * clock.deltaTime;
		}
		if (editor.preview_game.game_camera.y_direction == Direction::Up) {
			if (!editor.preview_game.enable_gravity) {
				camera_force.y -= editor.preview_game.camera_speed * clock.deltaTime;
			}
		}
		if (editor.preview_game.game_camera.y_direction == Direction::Down) {
			if (!editor.preview_game.enable_gravity) {
				camera_force.y += editor.preview_game.camera_speed * clock.deltaTime;
			}
		}
		if (editor.preview_game.enable_gravity) {
			camera_force.y += editor.preview_game.camera_speed * editor.preview_game.game_camera.time;
			if (editor.preview_game.game_camera.in_air) {
				camera_force.y -= 1 * clock.deltaTime;
			}
		}

		bool add_time = true;
		for (auto d : dynamic_walls) {
	 		for (auto s : static_walls) {
				int x1 = d->x + editor.preview_game.game_camera.x + camera_force.x;
				int y1 = d->y + editor.preview_game.game_camera.y + camera_force.y;
				int x2 = d->x + d->w + editor.preview_game.game_camera.x + camera_force.x;
				int y2 = d->y + d->h + editor.preview_game.game_camera.y + camera_force.y;
				bool phase = false;
				if (y1 >= s->y && y1 <= s->y + s->h) {
					if (x1 - camera_force.x >= s->x && x1 - camera_force.x <= s->x + s->w || x2 - camera_force.x >= s->x && x2 - camera_force.x <= s->x + s->w || (x1 - camera_force.x <= s->x && x2 - camera_force.x >= s->x + s->w)) {
						camera_force.y = 0;
						if (editor.preview_game.enable_gravity) {
							editor.preview_game.game_camera.time = 0;
					 		add_time = false;
							editor.preview_game.game_camera.in_air = false;
							editor.preview_game.game_camera.up = false;
							editor.preview_game.game_camera.y_direction = Direction::Down;
						}
						else {
							phase = true;
						}
					}
				}
				if (y2 <= s->y + s->h && y2 >= s->y) {
					if (x1 - camera_force.x >= s->x && x1 - camera_force.x <= s->x + s->w || x2 - camera_force.x >= s->x && x2 - camera_force.x <= s->x + s->w || (x1 - camera_force.x <= s->x && x2 - camera_force.x >= s->x + s->w)) {
						camera_force.y = 0;
						if (editor.preview_game.enable_gravity) {
							editor.preview_game.game_camera.time = 0;
							editor.preview_game.game_camera.y_direction= Direction::None;
							editor.preview_game.game_camera.in_air = false;
							can_jump = true;
						}
						else {
							phase = true;
						}
					}
				}
				if (camera_force.y == 0 && phase) {
					if (x2 >= s->x && x2 <= s->x + s->w) {
						camera_force.y = 0;
					}
					if (x1 <= s->x + s->w && x1 >= s->x) {
						camera_force.y = 0;
					}
				}
				else {
					if (x2 >= s->x && x2 <= s->x + s->w) {
						if (y1 >= s->y && y1 <= s->y + s->h || y2 > s->y && y2 <= s->y + s->h || (y1 <= s->y && y2 >= s->y + s->h)) {
							camera_force.x = 0;
						}
					}
					if (x1 <= s->x + s->w && x1 >= s->x) {
						if (y1 >= s->y && y1 <= s->y + s->h || y2 > s->y && y2 <= s->y + s->h || (y1 <= s->y && y2 >= s->y + s->h)) {
							camera_force.x = 0;
						}
					}
				}
			}
		}
		if (add_time) {
			editor.preview_game.game_camera.time += (editor.preview_game.gravity_force / 10.0) * 0.03 * clock.deltaTime;
		}
		editor.preview_game.game_camera.x += camera_force.x;
		editor.preview_game.game_camera.y += camera_force.y;
		for (auto layer : editor.preview_game.layers) {
			layer.Draw(AppRenderer, &editor.preview_game);
		}
		for (int x = 0; x < editor.preview_game.cp_LChain_data.size(); x++) {
			editor.preview_game.cp_LChain_data[x].InterpretChain(event, &editor.preview_game, this->clock, AppRenderer);
			editor.preview_game.recersion_count = 0;
		}
		int dir = 0;
		if (editor.preview_game.game_camera.y_direction == Direction::Up) {
			dir = 1;
		}
		else {
			dir = 0;
		}
		editor.displayCounter(AppRenderer, "%data%", dir);
		clock.lastupdate = clock.current;
		SDL_RenderPresent(AppRenderer);
	}
}
#pragma warning(pop);