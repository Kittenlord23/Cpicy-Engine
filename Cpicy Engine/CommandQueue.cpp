#include "CommandQueue.h"
#include "Editor.h"

void CommandQueue::evaluate_expression(Game* game, Editor* editor)
{
	if (this->commands.back().find("Add Rectangle: ") != std::string::npos) {
		std::vector<std::string> args = split(commands.back(), ":");
		GRect r;
		if (args.size() == 6) {
			try {
				r.x = std::stoi(args[1]);
				r.y = std::stoi(args[2]);
				r.w = std::stoi(args[3]);
				r.h = std::stoi(args[4]);
				r.r = editor->color[0];
				r.g = editor->color[1];
				r.b = editor->color[2];
				r.dynamic_static = editor->creation_dynamic_state;
				r.layer_number = editor->current_layer_number;
				r.z_index = -1;
				r.id = args[5];
				game->cp_rect_data.push_back(r);
				game->layers[editor->current_layer_number - 1].AddGraphic(r);
			}
			catch (std::invalid_argument) {
				MessageBoxA(NULL, "Invalid argument", "Error", MB_OK | MB_ICONERROR);
			}
		}
		else {
			MessageBoxA(NULL, "Requires 5 arguments", "Error", MB_OK | MB_ICONERROR);
		}
	}
	else if (this->commands.back().find("Add Image: ") != std::string::npos) {
		std::vector<std::string> args = split(commands.back(), ":");
		if (args.size() == 7) {
			try {
				GTexture t;
				t.x = std::stoi(args[1]);
				t.y = std::stoi(args[2]);
				t.w = std::stoi(args[3]);
				t.h = std::stoi(args[4]);
				t.texture = editor->current_texture;
				t.rotation_angle = std::stoi(args[5]);
				t.layer_number = editor->current_layer_number;
				t.z_index = -1;
				t.dynamic_static = editor->creation_dynamic_state;
				t.id = args[6];
				game->cp_texture_data.push_back(t);
				game->layers[editor->current_layer_number - 1].AddGraphic(t);
			}
			catch (std::invalid_argument) {
				MessageBoxA(NULL, "Invalid argument", "Error", MB_OK | MB_ICONERROR);
			}
		}
		else {
			MessageBoxA(NULL, "Requires 6 arguments", "Error", MB_OK | MB_ICONERROR);
		}
	}
	else if (this->commands.back().find("Switch Active Texture: ") != std::string::npos) {
		std::vector<std::string> args = split(commands.back(), ":");
		if (args.size() != 2) {
			for (int x = 0; x < game->loaded_textures.size(); x++) 
			{
				if (game->loaded_textures[x].name == args[1]) {
					editor->current_texture.content = &game->loaded_textures[x].content;
				}
			}
		}
	}
}

void CommandQueue::push_queue(std::string in)
{
	this->commands.push_back(in);
}

void CommandQueue::flush_queue(Game* game, Editor* editor)
{	
	for (int x = 0; x < commands.size(); x++) {
		evaluate_expression(game, editor);
		pop_queue();
	}
}

void CommandQueue::pop_queue()
{
	this->commands.pop_back();
}

CommandQueue::CommandQueue()
{
	this->commands = {};
}
