#pragma once
#include "ScrollBar.h"
#include <vector>
#include "Kiran.h"
#include "Shapes.h"
#include "Game.h"
#include "Camera.h"
#include "CompileTimeError.h"
#include "CommandQueue.h"

#define SPICY_STATIC 0
#define SPICY_DYNAMIC 1
#define WHITE 91
#define TAB "        "
#define COLORS 2

enum struct ProjectTemplateMode {
	SideScroller = 0,
	GodView = 1,
	Blank = 2,
};

struct Editor {
	bool creating_rectangle = false;
	bool creating_line = false;
	bool creating_image = false;
	bool creating_rect_wall = false;
	bool moving_DropDownBar = false;
	bool graphics_section_showing = false;
	bool interactive_section_showing = false;
	bool text_editor_section_showing = false;
	bool file_section_showing = false;
	bool edit_section_showing = false;
	bool view_section_showing = false;
	bool show_error_section = true;
	bool creation_mode = SPICY_STATIC;
	int color[3] = { 255, 255, 255 };
	std::vector<Point> creation_points = {};
	ScrollBar DropDownBar = ScrollBar();
	float check = 0;
	ProjectTemplateMode project_template_mode = ProjectTemplateMode::Blank;
	std::vector<SDL_Texture*> WhiteText = {};
	std::vector<std::string> undo_stack = {};
	std::vector<std::string> redo_stack = {};
	std::vector<GRect> redo_stack_rect_memory = {};
	std::vector<GRect> delete_undo_rect_memory = {};
	std::vector<GTexture> delete_undo_texture_memory = {};
	std::vector<GTexture> redo_stack_texture_memory = {};
	std::vector<GRWall> redo_stack_rect_wall_memory = {};
	std::vector<GRWall> delete_undo_rect_wall_memory = {};
	std::vector<SDL_Texture*> view_image_textures = {};
	std::vector<SDL_Texture*> view_file_textures = {};
	std::vector<SDL_Texture*> Alphabet = {};
	int view_image_index = -1;
	int view_file_index = -1;
	std::vector<std::string> file_paths = {};
	Camera editor_camera;
	bool moving_camera = false;
	bool creation_dynamic_state = SPICY_STATIC;
	bool collapse_file_section;
	bool collapse_image_section;
	bool deleting_object;
	Texture current_texture;
	int current_texture_index = 0;
	int current_texture_name_length = 4;
	int graphic_index = 0;
	int logic_section_mode = 0;
	std::string current_id;
	int line_number = 0;
	int line_spacing = 0;
	std::string r = "ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz1234567890,:!@#$%^&*()-=;[]{}|~`+._<>";
	bool editing_editor_text;
	std::vector<std::string> lines;
	std::vector<CompileTimeErrorHandel> errors;
	int link_custom_id = 0;
	bool key_press_key_dropdown_showing = false;
	int key_press_key_dropdown_id = 0;
	unsigned short int  current_chain_id = 0;
	int cursor_spacing = 0;
	int cursor_line_number = 0;
	int current_layer_number = 1;
	CommandQueue cmdQueue = CommandQueue();
	// Function prototype
	Game preview_game{};
	void undo(Game* game);
	void swapCameraRelation();
	void redo(Game* game);
	void updateRedoStack();
	void openFiles(std::string path, SDL_Renderer* renderer);
	void swapDeleteState(Game* game);
	void reciveFileData(std::string path);
	void incrementIdValue();
	void saveFileData(std::string path);
	void drawText(std::string text, SDL_Renderer* renderer, int color);
	void importFont(Game* game);
	void LoadLogicFile(Game* game);
	void SaveLogicFile(Game* game);
	void RenderErrors(SDL_Renderer* renderer);
	void displayCounter(SDL_Renderer* renderer, std::string input, int value, int x = 100, int y =  100);
};