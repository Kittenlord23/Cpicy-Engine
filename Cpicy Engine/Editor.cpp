#include "Editor.h"
#include "Game.h"
#include "Chain.h"
#include "SDL+.h"
#include <SDL.h>
#include <filesystem>
#include "Compiler.h"
void Editor::updateRedoStack() {
	redo_stack.clear();
	redo_stack_rect_memory.clear();
}
void Editor::undo(Game* game) {
	if (undo_stack.size() > 0) {
		std::string action = undo_stack.back();
		if (action == "rectangle") {
			undo_stack.pop_back();
			game->layers[game->cp_rect_data.back().layer_number - 1].chain.pop_back();
			GRect remember = game->cp_rect_data.back();
			game->cp_rect_data.pop_back();
			redo_stack.push_back("rectangle");
			redo_stack_rect_memory.push_back(remember);
		}
		if (action == "texture") {
			undo_stack.pop_back();
			game->layers[game->cp_texture_data.back().layer_number - 1].chain.pop_back();
			GTexture remember = game->cp_texture_data.back();
			game->cp_texture_data.pop_back();
			redo_stack.push_back("texture");
			redo_stack_texture_memory.push_back(remember);
		}
		if (action == "rect wall") {
			undo_stack.pop_back();
			GRWall remember = game->cp_rect_wall_data.back();
			game->cp_rect_wall_data.pop_back();
			redo_stack.push_back("rect wall");
			redo_stack_rect_wall_memory.push_back(remember);
		}
		if (action == "d rect") {
			undo_stack.pop_back();
			game->cp_rect_data.push_back(delete_undo_rect_memory.back());
			game->layers[game->cp_rect_data.back().layer_number - 1].AddGraphic(game->cp_rect_data.back());
			delete_undo_rect_memory.pop_back();
		}
		if (action == "d texture") {
			undo_stack.pop_back();
			game->cp_texture_data.push_back(delete_undo_texture_memory.back());
			game->layers[game->cp_texture_data.back().layer_number - 1].AddGraphic(game->cp_texture_data.back());
			delete_undo_texture_memory.pop_back();
		}
		if (action == "d rect wall") {
			undo_stack.pop_back();
			game->cp_rect_wall_data.push_back(delete_undo_rect_wall_memory.back());
			delete_undo_rect_wall_memory.pop_back();
		}
	}
}
void Editor::redo(Game* game) {
	if (redo_stack.size() > 0) {
		std::string action = redo_stack.back();
		if (action == "rectangle") {
			redo_stack.pop_back();
			game->addRect(redo_stack_rect_memory.back());
			std::cout << game->layers[game->cp_rect_data.back().layer_number - 1].chain.size() << std::endl;;
			game->layers[game->cp_rect_data.back().layer_number - 1].AddGraphic(game->cp_rect_data.back());
			std::cout << game->layers[game->cp_rect_data.back().layer_number - 1].chain.size() << std::endl;;
			redo_stack_rect_memory.pop_back();
			undo_stack.push_back("rectangle");
		}
		if (action == "texture") {
			redo_stack.pop_back();
			game->addTexture(redo_stack_texture_memory.back());
			game->layers[game->cp_texture_data.back().layer_number - 1].AddGraphic(game->cp_texture_data.back());
			redo_stack_texture_memory.pop_back();
			undo_stack.push_back("texture");
		}
		if (action == "rect wall") {
			redo_stack.pop_back();
			game->cp_rect_wall_data.push_back(redo_stack_rect_wall_memory.back());
			redo_stack_rect_wall_memory.pop_back();
			undo_stack.push_back("rect wall");
		}
	}
}
void Editor::swapCameraRelation() {
	if (creation_dynamic_state == SPICY_STATIC) {
		creation_dynamic_state = SPICY_DYNAMIC;
	}
	else {
		creation_dynamic_state = SPICY_STATIC;
	}
}
void Editor::openFiles(std::string path, SDL_Renderer* renderer) {
	Alphabet.clear();
	for (int x = 0; x < r.size(); x++) {
		SDL_Color Black{ 0, 0, 0 };
		Alphabet.push_back(GPU_LoadText(std::string(1, r[x]) , renderer, 24, GetExecutablePath() + "Extern\\Fonts\\monospace.ttf", Black));
	}
	for (int x = 0; x < r.size(); x++) {
		SDL_Color White{ 255, 255, 255 };
		Alphabet.push_back(GPU_LoadText(std::string(1, r[x]), renderer, 24, GetExecutablePath() + "Extern\\Fonts\\monospace.ttf", White));
	}
	view_file_textures.clear();
	file_paths.clear();
	auto files = GetFiles(path);
	for (int x = 0; x < files.size(); x++) {
		if (split(files[x], ".").back() == "spice") {
			view_file_textures.push_back(GPU_LoadText(files[x], renderer, floor(GetDesktopResolution('y') / 77),GetExecutablePath() + "Extern\\Fonts\\arial.ttf"));
			file_paths.push_back(files[x]);
		}
	}
}

void Editor::swapDeleteState(Game* game)
{
	if (deleting_object) {
		deleting_object = false;
		game->Sort();
	}
	else {  
		deleting_object = true;
	}
}

void Editor::reciveFileData(std::string path) {
	std::ifstream File(path);	 	// Create a ifstream file based on the path
	std::string file_data;			// Create an empty string for the total data
	std::getline(File, file_data);		// Fill the string with the data
	current_id = replace(replace(split(file_data, " -> ")[0], "current id[", ""), "]", "").c_str();
}

void Editor::incrementIdValue() {
	{
		int int_value = std::stoi(current_id);
		int digits = int_value > 0 ? (int)log10((double)int_value) + 1 : 1; \
			int_value++;
		int num_of_0s = 8 - digits;
		current_id.clear();
		for (int x = 0; x < num_of_0s; x++) {
			current_id += "0";
		}
		current_id += std::to_string(int_value);
	}
}
void Editor::saveFileData(std::string path) {
	std::ofstream File(path);
	File << "current id[" + current_id + "]" + " -> end <->\n";
}

void Editor::drawText(std::string text, SDL_Renderer* renderer, int color)
{
	line_spacing = 0;
	int w;
	int h;
	for (int x = 0; x < text.size(); x++) {
		for (int y = 0; y < Alphabet.size() / COLORS; y++) {
			if (r[y] == text[x]) {
				SDL_QueryTexture(Alphabet[y + color], NULL, NULL, &w, &h);
				SDL_Rect rect{ GetDesktopResolution('x') * 0.2 + line_spacing * 15 +editor_camera.x, GetDesktopResolution('x') * 0.2f + line_number * 24 + editor_camera.y, w, h};
				SDL_RenderCopy(renderer, Alphabet[y + color], NULL, &rect);
				line_spacing++;
			}
		}
	}
	
	auto digits = std::to_string(line_number); 
	SDL_QueryTexture(Alphabet[0], NULL, NULL, &w, &h);
	SDL_Rect number_rect{ GetDesktopResolution('x') * 0.18 + editor_camera.x, GetDesktopResolution('x') * 0.2 + editor_camera.y + line_number * 24, w, h };
	std::reverse(digits.begin(), digits.end());
	for (auto digit : digits) {
		for (int x = 53; x < 63; x++) {
			if (r[x] == digit) {
				SDL_RenderCopy(renderer, Alphabet[x + color], NULL, &number_rect);
				number_rect.x -= w;
			}
		}
	}
	line_number++;
}
void Editor::LoadLogicFile(Game* game) {
	std::ifstream file(replace(game->getWorkingPath(), split(game->getWorkingPath(), "\\").back(), split(split(game->getWorkingPath(), "\\").back(), ".").front() + ".source")); //file just has some sentences
	std::string buffer;
	std::string data = "";
	while (getline(file, buffer))
	{
		data += buffer + "%NEWLINE%";
	}
	lines = split(data, "%NEWLINE%");
}
void Editor::SaveLogicFile(Game* game) {
	std::ofstream file(replace(game->getWorkingPath(), split(game->getWorkingPath(), "\\").back(), split(split(game->getWorkingPath(), "\\").back(), ".").front() + ".source"));
	for (auto line : lines) {
		file << line << "\n";
	}
}
void Editor::RenderErrors(SDL_Renderer* renderer)
{
	line_spacing = 0;
	for (auto t = errors.begin(); t != errors.end(); ++t) {
		if (t->error_number == MISSING_START_ERROR) {
			std::string message = "Missing required label: On Start";
			for (auto character : message) {
				for (int x = 0; x < r.size(); x++) {
					int w;
					int h;
					SDL_QueryTexture(Alphabet[x], NULL, NULL, &w, &h);
					SDL_Rect character_rect{ GetDesktopResolution('x') * 0.2 + line_spacing * 15, GetDesktopResolution('y') * 0.82 + line_number * 24, w, h };
					if (character == r[x]) {
						SDL_RenderCopy(renderer, Alphabet[x + WHITE - 1], NULL, &character_rect);
						line_spacing++;
					}
				}
			}
		}
		if (t->error_number == MISSING_EVENT_ERROR) {
			std::string message = "Missing required label: On Event" ;
			for (auto character : message) {
				for (int x = 0; x < r.size(); x++) {
					int w;
					int h;
					SDL_QueryTexture(Alphabet[x], NULL, NULL, &w, &h);
					SDL_Rect character_rect{ GetDesktopResolution('x') * 0.2 + line_spacing * 15, GetDesktopResolution('y') * 0.82 + line_number * 24, w, h };
					if (character == r[x]) {
						SDL_RenderCopy(renderer, Alphabet[x + WHITE - 1], NULL, &character_rect);
						line_spacing++;
					}
				}
			}
		}
		if (t->error_number == MISSING_LOOP_ERROR) {
			std::string message = "Missing required label: On Loop";
			for (auto character : message) {
				for (int x = 0; x < r.size(); x++) {
					int w;
					int h;
					SDL_QueryTexture(Alphabet[x], NULL, NULL, &w, &h);
					SDL_Rect character_rect{ GetDesktopResolution('x') * 0.2 + line_spacing * 15, GetDesktopResolution('y') * 0.82 + line_number * 24, w, h };
					if (character == r[x]) {
						SDL_RenderCopy(renderer, Alphabet[x + WHITE - 1], NULL, &character_rect);
						line_spacing++;
					}
				}
			}
		}
		if (t->error_number == UNKNOWN_CAUSETYPE_ERROR) {
			std::string message = "Unknown method of execution: " + replace(t->line_content, TAB, "");
			for (auto character : message) {
				for (int x = 0; x < r.size(); x++) {
					int w;
					int h;
					SDL_QueryTexture(Alphabet[x], NULL, NULL, &w, &h);
					SDL_Rect character_rect{ GetDesktopResolution('x') * 0.2 + line_spacing * 15, GetDesktopResolution('y') * 0.82 + line_number * 24, w, h };
					if (character == r[x]) {
						SDL_RenderCopy(renderer, Alphabet[x + WHITE - 1], NULL, &character_rect);
						line_spacing++;
					}
				}
			}
		}
		if (t->error_number == NOT_AN_INT_CAUSEVALUE_ERROR) {
			std::string message = "Expected a number here inbetween[]: " + replace(t->line_content, TAB, "");
			for (auto character : message) {
				for (int x = 0; x < r.size(); x++) {
					int w;
					int h;
					SDL_QueryTexture(Alphabet[x], NULL, NULL, &w, &h);
					SDL_Rect character_rect{ GetDesktopResolution('x') * 0.2 + line_spacing * 15, GetDesktopResolution('y') * 0.82 + line_number * 24, w, h };
					if (character == r[x]) {
						SDL_RenderCopy(renderer, Alphabet[x + WHITE - 1], NULL, &character_rect);
						line_spacing++;
					}
				}
			}
		}
		if (t->error_number == NOT_A_KEY_CAUSEVALUE_ERROR) {
			std::string message = "Expected a key identifier here inbetween[]: " + replace(t->line_content, TAB, "");
			for (auto character : message) {
				for (int x = 0; x < r.size(); x++) {
					int w;
					int h;
					SDL_QueryTexture(Alphabet[x], NULL, NULL, &w, &h);
					SDL_Rect character_rect{ GetDesktopResolution('x') * 0.2 + line_spacing * 15, GetDesktopResolution('y') * 0.82 + line_number * 24, w, h };
					if (character == r[x]) {
						SDL_RenderCopy(renderer, Alphabet[x + WHITE - 1], NULL, &character_rect);
						line_spacing++;
					}
				}
			}
		}
		if (t->error_number == UNEXPECTED_TOKEN_VARIABLE_ERROR) {
			std::string message = "Illegal variable name: " + replace(t->line_content, TAB, "");
			for (auto character : message) {
				for (int x = 0; x < r.size(); x++) {
					int w;
					int h;
					SDL_QueryTexture(Alphabet[x], NULL, NULL, &w, &h);
					SDL_Rect character_rect{ GetDesktopResolution('x') * 0.2 + line_spacing * 15, GetDesktopResolution('y') * 0.82 + line_number * 24, w, h };
					if (character == r[x]) {
						SDL_RenderCopy(renderer, Alphabet[x + WHITE - 1], NULL, &character_rect);
						line_spacing++;
					}
				}
			}
		}
		if (t->error_number == NOT_AN_INT_CAUSEVALUE_ERROR) {
			std::string message = "Could not evaluate as numbers where expected: " + replace(t->line_content, TAB, "");
			for (auto character : message) {
				for (int x = 0; x < r.size(); x++) {
					int w;
					int h;
					SDL_QueryTexture(Alphabet[x], NULL, NULL, &w, &h);
					SDL_Rect character_rect{ GetDesktopResolution('x') * 0.2 + line_spacing * 15, GetDesktopResolution('y') * 0.82 + line_number * 24, w, h };
					if (character == r[x]) {
						SDL_RenderCopy(renderer, Alphabet[x + WHITE - 1], NULL, &character_rect);
						line_spacing++;
					}
				}
			}
		}
		if (t->error_number == 8) {
			std::string message = "Unbalance of (begin function) and (end function) lines";
			for (auto character : message) {
				for (int x = 0; x < r.size(); x++) {
					int w;
					int h;
					SDL_QueryTexture(Alphabet[x], NULL, NULL, &w, &h);
					SDL_Rect character_rect{ GetDesktopResolution('x') * 0.2 + line_spacing * 15, GetDesktopResolution('y') * 0.82 + line_number * 24, w, h };
					if (character == r[x]) {
						SDL_RenderCopy(renderer, Alphabet[x + WHITE - 1], NULL, &character_rect);
						line_spacing++;
					}
				}
			}
		}
		line_number++;
		line_spacing = 0;
	}
}

void Editor::importFont(Game* game) {
	std::string path = GetFilePath("Font\000*.ttf\000");
	if (split(split(path, "\\").back(), ".").back().find("ttf") == std::string::npos) {
		MessageBox(NULL, "Specified file is not a font(ttf)", "Error", MB_ICONERROR);
		return;
	}
	std::string to = "";
	if (game->getWorkingPath().find(".spice") != std::string::npos) {
		to = replace(game->getWorkingPath(), split(game->getWorkingPath(), "\\").back(), "") + "Fonts\\";
	}
	else {
		to = game->getWorkingPath();
	}
	try {
		std::filesystem::copy_file(path, to + split(path, "\\").back());
	}
	catch (std::filesystem::filesystem_error e) {
		MessageBoxA(NULL, e.what(), "Error", MB_OK | MB_ICONERROR);
	}
}
void Editor::displayCounter(SDL_Renderer* renderer, std::string input, int value, int x_location, int y_loaction)
{
	input = replace(input, "%data%", std::to_string(value));
	for (int y = 0; y < input.size(); y++) {
		for (int x = 0; x < r.size(); x++) {
			int w;
			int h;
			SDL_QueryTexture(Alphabet[x], NULL, NULL, &w, &h);
			SDL_Rect character_rect{ x_location + w * y, y_loaction, w, h };
			if (input[y]== r[x]) {
				SDL_RenderCopy(renderer, Alphabet[x + WHITE - 1], NULL, &character_rect);
			}
		}
	}
}