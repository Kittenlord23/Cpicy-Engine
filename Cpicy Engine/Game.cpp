#include "Game.h"
#include "SDL+.h"
#include "App.h"
#include "Chain.h"
#include <assert.h>

std::string Game::BoolStr(bool boolean) {
	if (boolean == true) {
		return "true";
	}
	else {
		return "false";
	}
}
std::string Game::getTriggerCauseAsString(TriggerType trigger) {
	if (trigger.cause == "key press") {
		return GetStringFromKey(trigger.cause_value.key);
	}
	if (trigger.cause == "key release") {
		return GetStringFromKey(trigger.cause_value.key);
	}
	if (trigger.cause == "pushed") {
		return "none";
	}
	if (trigger.cause == "time passed") {
		return std::to_string(trigger.cause_value.timer.dest);
	}
	if (trigger.cause == "in air") {
		return "none";
	}
	return "none";
}
void Game::logError(std::string link_command, std::string description, std::string example)
{
	if (enable_errors) {
		std::cout << "-----------------------------------\n";
		std::cout << "Error: " << link_command << std::endl;
		std::cout << description << std::endl;
		std::cout << "Example: " << example << std::endl;
		std::cout << "-----------------------------------\n";
	}
}
void Game::clear() {
	cp_rect_data.clear();
	cp_texture_data.clear();
	cp_rect_wall_data.clear();
	cp_EChain_data.clear();
	cp_LChain_data.clear();
	layers.clear();
	current_working_path = "";
	game_template = "";
}
void Game::addRect(GRect rect) {
	cp_rect_data.push_back(rect);
}
CauseValue Game::GetCauseValueFromString(std::string cause, std::string input) {
	CauseValue t{};
	if (cause == "key press") {
		try {
			t.key = GetKeyFromString(input);
			return t;
		}
		catch (int e) {
			if (e == 5) {
				throw 5;
			}
		}
	}
	if (cause == "key release") {
		try {
			t.key = GetKeyFromString(input);
			return t;
		}
		catch (int e) {
			if (e == 5) {
				throw 5;
			}
		}
	}
	if (cause == "time passed") {
		t.timer.current = 0;
		try {
			t.timer.dest = std::stoi(input);
		}
		catch (std::invalid_argument) {
			throw 4;
		}
		return t;
	}
	if (cause == "in air") {
		t.unused = true;
		return t;
	}
	if (cause == "pushed" || cause == "none") {
		t.unused = true;
		return t;
	}
	else {
		throw 3;
	}
}
void Game::addTexture(GTexture texture)
{
	cp_texture_data.push_back(texture);
}
bool Game::setWorkingPath(std::string path, SDL_Renderer* AppRenderer) {
	clear();
	current_working_path = path;	// Set the string working path
	std::ifstream file(path);	 	// Create a ifstream file based on the path
	std::string file_data;			// Create an empty string for the total data
	std::getline(file, file_data);		// Fill the string with the data
	loadTextures(replace(getWorkingPath(), split(getWorkingPath(), "\\").back(), "") + "Images\\", AppRenderer);
	loadLogicFile(replace(path, split(path, "\\").back(), "") + replace(split(path, "\\").back(), ".spice", "") + ".spiceL");
	// Split the string down into bae paramaters that are used to sort
	// them into releveant data for graphic drawing, collision, ect.

	game_template = split(file_data, "->")[0];
	max_frame_rate = std::stoi(replace(replace(replace(replace(split(file_data, " -> ")[2], "type_setting_frame_delay", ""), "[", ""), "]", ""), " ", ""));
	for (int x = 0; x < std::stoi(replace(replace(replace(replace(split(file_data, " -> ")[3], "layers", ""), "[", ""), "]", ""), " ", ""));  x++) {
		Layer layer = Layer(x + 1);
		layers.push_back(layer);
	}
	std::vector<std::string> rect_data = split(replace(replace(replace(split(file_data, " -> ")[5], "type_rect", ""), "[", ""), "]", ""), "::");
	std::vector<std::string> texture_data = split(replace(replace(replace(split(file_data, " -> ")[7], "type_image", ""), "[", ""), "]", ""), "::");
	std::vector<std::string> rect_wall_data = split(replace(replace(replace(split(file_data, " -> ")[8], "type_rect_wall", ""), "[", ""), "]", ""), "::");
	// For each string inside the vector of strings,
	// begin assigning their values to a rectangle
	// which will be added to the final rectangle list

	if (rect_data.size() != 0) {
		// We start from 0 and compare to size - 1 because during the saving process,
		// the seperator token "::" is added regardless if there is an actualy valid object token
		// so we have to ignore the last one since it is not actualy there.

		for (int x = 0; x < rect_data.size() - 1; x++) {
			GRect rect = GRect(); // Create the empty rectangle with uninitialized values

			// Here we begin spliting the strings of rectangles into
			// numerical values that can be proccsesd by the Cpicy
			// interpreter
			try {
				// x1 value
				rect.x = std::stoi(split(rect_data[x], ",")[0]);
				// y1 value
				rect.y = std::stoi(split(rect_data[x], ",")[1]);
				// width value
				rect.w = std::stoi(split(rect_data[x], ",")[2]);
				// height value
				rect.h = std::stoi(split(rect_data[x], ",")[3]);
				// Red value
				rect.r = std::stoi(split(rect_data[x], ",")[4]);
				// Green value
				rect.g = std::stoi(split(rect_data[x], ",")[5]);
				// Blue value
				rect.b = std::stoi(split(rect_data[x], ",")[6]);
				// dynamic value, which is used to tell if a shape should move
				// relitive to the camera position, or just stay still
				rect.dynamic_static = std::stoi(split(rect_data[x], ",")[7]);
				rect.id = split(rect_data[x], ",")[8];
				rect.layer_number = std::stoi(split(rect_data[x], ",")[9]);
				rect.z_index = std::stoi(split(rect_data[x], ",")[10]);
				// Finaly, add the rect with all of it's initlized values to
				// actual rectangle vector, with their own type that can be
				// used to read and write to

				addRect(rect);
			}

			// If the user edits the source code for spicy projects directly,
			// then std::stoi will throw an error. Catch it here and tell
			// the user they are dumb dumb.

			catch (std::invalid_argument) {
				MessageBoxA(NULL, "Failed to complete stage 1 compilation. Check for syntax errors or other missing/invalid characters", "Cpicy Engine", MB_OK | MB_ICONERROR);

				// Tell the event handeler we are not readry to move
				// to the next step
				file.close();
				return false;
			}
			catch (std::bad_alloc) {
				MessageBoxA(NULL, "Failed to complete stage 1 compilation. Check for syntax errors or other missing/invalid characters", "Cpicy Engine", MB_OK | MB_ICONERROR);

				// Tell the event handeler we are not readry to move
				// to the next step
				file.close();
				return false;
			}
		}
	}
	if (texture_data.size() != 0) {
		for (int x = 0; x < texture_data.size() - 1; x++) {
			GTexture texture = GTexture();

			try {
				texture.x = std::stoi(split(texture_data[x], ",")[0]);
				texture.y = std::stoi(split(texture_data[x], ",")[1]);
				texture.w = std::stoi(split(texture_data[x], ",")[2]);
				texture.h = std::stoi(split(texture_data[x], ",")[3]);
				texture.rotation_angle = std::stoi(split(texture_data[x], ",")[4]);
				texture.dynamic_static = std::stoi(split(texture_data[x], ",")[5]);
				for (int z = 0; z < loaded_textures.size(); z++) {
					if (split(texture_data[x], ",")[6] == loaded_textures[z].name) {
						texture.texture.content = &loaded_textures[z].content;
						texture.texture.name = loaded_textures[z].name;
						break;
					}
				}
				texture.id = split(texture_data[x], ",")[7];
				texture.layer_number = std::stoi(split(texture_data[x], ",")[8]);
				texture.z_index = std::stoi(split(texture_data[x], ",")[9]);
				addTexture(texture);
			}
			catch (std::invalid_argument) {
				MessageBoxA(NULL, "Failed to complete stage 1 compilation. Check for syntax errors or other missing/invalid characters", "Cpicy Engine", MB_OK | MB_ICONERROR);

				// Tell the event handeler we are not readry to move
				// to the next step
				file.close();
				return false;
			}
			catch (std::bad_alloc) {
				MessageBoxA(NULL, "Failed to complete stage 1 compilation. Check for syntax errors or other missing/invalid characters", "Cpicy Engine", MB_OK | MB_ICONERROR);

				// Tell the event handeler we are not readry to move
				// to the next step
				file.close();
				return false;
			}
		}
	}
	if (rect_wall_data.size() != 0) {
		for (int x = 0; x < rect_wall_data.size() - 1; x++) {
			GRWall wall = GRWall();
			try {
				wall.x = std::stoi(split(rect_wall_data[x], ",")[0]);
				wall.y = std::stoi(split(rect_wall_data[x], ",")[1]);
				wall.w = std::stoi(split(rect_wall_data[x], ",")[2]);
				wall.h = std::stoi(split(rect_wall_data[x], ",")[3]);
				wall.dynamic_static = std::stoi(split(rect_wall_data[x], ",")[4]);
				wall.id = split(rect_wall_data[x], ",")[5];
				cp_rect_wall_data.push_back(wall);
			}
			catch (std::invalid_argument) {
				MessageBoxA(NULL, "Failed to complete stage 1 compilation. Check for syntax errors or other missing/invalid characters", "Cpicy Engine", MB_OK | MB_ICONERROR);

				// Tell the event handeler we are not readry to move
				// to the next step
				file.close();
				return false;
			}
			catch (std::bad_alloc) {
				MessageBoxA(NULL, "Failed to complete stage 1 compilation. Check for syntax errors or other missing/invalid characters", "Cpicy Engine", MB_OK | MB_ICONERROR);

				// Tell the event handeler we are not readry to move
				// to the next step
				file.close();
				return false;
			}
		}
	}
	// Close the file

	file.close();
	this->Sort();
	// Return confirmation everything went good on this end

	return true;
}
std::string Game::getWorkingPath() {
	return current_working_path;
}
void  Game::saveProject() {
	// Open the file from the working path
	std::ofstream file(getWorkingPath());

	// Clean the file of old data

	file.clear();
	std::string data = "";
	data += game_template + "-> ";
	data += "type_setting_vsync[True] -> ";
	data += "type_setting_frame_delay[" + std::to_string(max_frame_rate) + "] -> ";
	data += "layers[" + std::to_string(layers.size()) + "] -> ";
	data += "type_text[] -> type_rect[";
	if (cp_rect_data.size() > 0) {
		for (int x = 0; x < cp_rect_data.size(); x++) {
			data += "["
				+ std::to_string(cp_rect_data[x].x) + ","
				+ std::to_string(cp_rect_data[x].y) + ","
				+ std::to_string(cp_rect_data[x].w) + ","
				+ std::to_string(cp_rect_data[x].h) + ","
				+ std::to_string(cp_rect_data[x].r) + ","
				+ std::to_string(cp_rect_data[x].g) + ","
				+ std::to_string(cp_rect_data[x].b) + ","
				+ std::to_string(cp_rect_data[x].dynamic_static) + ","
				+ cp_rect_data[x].id + "," 
				+ std::to_string(cp_rect_data[x].layer_number) + ","
				+ std::to_string(cp_rect_data[x].z_index) + 
				"]::"
				;
		}
	}
	data += "] -> ";
	data += "type_line[] -> ";
	data += "type_image[";
	if (cp_texture_data.size() > 0) {
		for (int x = 0; x < cp_texture_data.size(); x++) {
			data += "["
				+ std::to_string(cp_texture_data[x].x) + ","
				+ std::to_string(cp_texture_data[x].y) + ","
				+ std::to_string(cp_texture_data[x].w) + ","
				+ std::to_string(cp_texture_data[x].h) + ","
				+ std::to_string(cp_texture_data[x].rotation_angle) + ","
				+ std::to_string(cp_texture_data[x].dynamic_static) + ","
				+ cp_texture_data[x].texture.name + ","
				+ cp_texture_data[x].id + ","
				+ std::to_string(cp_texture_data[x].layer_number) + ","
				+ std::to_string(cp_texture_data[x].z_index) +
				"]::"
				;
		}
	}
	data += "] -> ";
	data += "type_rect_wall[";
	if (cp_rect_wall_data.size() > 0) {
		for (int x = 0; x < cp_rect_wall_data.size(); x++) {
			data += "["
				+ std::to_string(cp_rect_wall_data[x].x) + ","
				+ std::to_string(cp_rect_wall_data[x].y) + ","
				+ std::to_string(cp_rect_wall_data[x].w) + ","
				+ std::to_string(cp_rect_wall_data[x].h) + ","
				+ std::to_string(cp_rect_wall_data[x].dynamic_static) + ","
				+ cp_rect_wall_data[x].id +
				"]::"
				;
		}
	}
	data += "] -> end <->";
	file << data.c_str();
	file.close();
	data.clear();
	std::ofstream logic_file(replace(getWorkingPath(), split(getWorkingPath(), "\\").back(), "") + split(split(getWorkingPath(), "\\").back(), ".").front() + ".spiceL");
	for (int x = 0; x < integers.size(); x++) {
		data += "Int: " + integers[x].name + "=" + std::to_string(integers[x].value) + ";\n";
	}
	for (int x = 0; x < functions.size(); x++) {
		data += "function " + functions[x].name + ": ";
		for (int y = 0; y < functions[x].arg_names.size(); y++) {
			if (y != 0) {
				data += ":";
			}
			data += functions[x].arg_names[y];
		}
		data += " "; 
		data += "{";
		for (int y = 0; y < functions[x].commands.size(); y++) {
			if (y != functions[x].commands.size() - 1) {
				data += replace(functions[x].commands[y] + "@", ":", "^");
			}
			else {
				data += replace(functions[x].commands[y], ":", "^");
			}
		}
		data += "};\n";
	}
	for (int x = 0; x < cp_EChain_data.size(); x++) {
		data += "EChain:";
		for (int y = 0; y < cp_EChain_data[x].chain.size(); y++) {
			if (y != 0) {
				data += "::";
			}
			data += "Block[" + cp_EChain_data[x].chain[y].trigger.cause + "," + getTriggerCauseAsString(cp_EChain_data[x].chain[y].trigger) + "," + cp_EChain_data[x].chain[y].action + "," + cp_EChain_data[x].chain[y].send_data + "," + BoolStr(cp_EChain_data[x].chain[y].push) + "]";
		}
		data += ";\n";
	}
	for (int x = 0; x < cp_LChain_data.size(); x++) {
		data += "LChain:";
		for (int y = 0; y < cp_LChain_data[x].chain.size(); y++) {
			if (y != 0) {
				data += "::";
			}
			data += "Block[" + cp_LChain_data[x].chain[y].trigger.cause + "," + getTriggerCauseAsString(cp_LChain_data[x].chain[y].trigger) + "," + cp_LChain_data[x].chain[y].action + "," + cp_LChain_data[x].chain[y].send_data + "," + BoolStr(cp_LChain_data[x].chain[y].push) + "]";
		}
		data += ";";
	}
	logic_file << data.c_str();
	logic_file.close();
} 
void Game::loadTextures(std::string path, SDL_Renderer* renderer) {
	loaded_textures.clear();
	auto files = GetFiles(path);
	for (int x = 0; x < files.size(); x++) {
		if (split(files[x], ".").back() == "png") {
			LoadedTexture file;
			file.name = split(files[x], "\\").back();
			file.content = GPU_LoadTexture(replace(getWorkingPath(), split(getWorkingPath(), "\\").back(), "") + "Images\\" + files[x], renderer);
			loaded_textures.push_back(file);
		}
	}
}
void Game::loadLogicFile(std::string path) {
	std::ifstream file(path);
	std::string file_data((std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>());
	file_data = replace(file_data, "\n", "");
	std::vector<std::string> lines = split(file_data, ";");
	for (int x = 0; x < lines.size(); x++) {
		convertStringToLogic(lines[x]);
	}
}
void Game::convertStringToLogic(std::string str) {
	if (str.find("Int:") != std::string::npos) {
		std::string name = split(replace(replace(str, "Int:", ""), " ", ""), "=").front();
		int value = std::stoi(split(replace(replace(str, "Int:", ""), " ", ""), "=").back());
		integers.push_back(CpicyInteger(name, value, function_call_stack_count));
	}
	if (str.find("function ") != std::string::npos) {
		str.pop_back(); // remove ;
		UserFunction f;
		f.name = split(replace(str, "function ", ""), ":").front();
		auto args = split(replace(str, "function ", ""), ":");
		args.erase(args.begin()); // get rid of keyword function + name
		f.arg_names = args;
		auto t = split(replace(replace(replace(str, "function ", ""), "{", ""), "}", ""), ":").back();
		f.arg_names.front().erase(f.arg_names.front().begin());
		f.arg_names.back() = split(f.arg_names.back(), " {").front();
		t.erase(t.begin());
		f.commands = split(t, "@");
		int open = str.find_first_of("{");
		int close = str.find_last_of("}");
		std::string search = str.substr(open + 1, close - 1);
		f.commands = split(search, "@");
		for (auto command = f.commands.begin(); command != f.commands.end(); ++command) {
			*command = replace(*command, "^", ":");
		}
		functions.push_back(f);
	}
	if (str.find("EChain:") != std::string::npos) {
		std::vector<std::string> Blocks = split(replace(str, "EChain:", ""), "::");
		Chain this_chain;
		for (int x = 0; x < Blocks.size(); x++) {
			std::vector<std::string> block = split(replace(replace(replace(Blocks[x], "Block", ""), "]", ""), "[", ""), ",");
			bool push;
			std::string action;
			std::string send_data;
			TriggerType trigger_type;
			if (block[4] == "true") {
				push = true;
			}
			else {
				push = false;
			}
			send_data = block[3];
			action = block[2];
			trigger_type.cause = block[0];
			trigger_type.cause_value = GetCauseValueFromString(block[0], block[1]);
			Link link;
			link.action = action;
			link.push = push;
			link.send_data = send_data;
			link.trigger = trigger_type;
			this_chain.chain.push_back(link);
		}
		cp_EChain_data.push_back(this_chain);
	}
	if (str.find("LChain:") != std::string::npos) {
		std::vector<std::string> Blocks = split(replace(str, "LChain:", ""), "::");
		Chain this_chain;
		for (int x = 0; x < Blocks.size(); x++) {
			std::vector<std::string> block = split(replace(replace(Blocks[x], "Block[", ""), "]", ""), ",");
			bool push;
			std::string action;
			std::string send_data;
			TriggerType trigger_type;
			if (block[4] == "true") {
				push = true;
			}
			else {
				push = false;
			}
			send_data = block[3];
			action = block[2];
			trigger_type.cause = block[0];
			trigger_type.cause_value = GetCauseValueFromString(block[0], block[1]);
			Link link;
			link.action = action;
			link.push = push;
			link.send_data = send_data;
			link.trigger = trigger_type;
			this_chain.chain.push_back(link);
		}
		cp_LChain_data.push_back(this_chain);
	}
}
void Game::scope() {
	void_integers.push_back(0);
	void_integer_pointers.push_back(0);
	void_userclasses.push_back(0);
	void_userclass_pointers.push_back(0);
}
void Game::unscope() {
	for (int x = 0; x < void_integers.back(); x++) {
		integers.pop_back();
	}
	void_integers.pop_back();
	for (int x = 0; x < void_integer_pointers.back(); x++) {
		integer_pointers.pop_back();
	}
	void_integer_pointers.pop_back();
	for (int x = 0; x < void_userclasses.back(); x++) {
		instances.pop_back();
	}
	void_userclasses.pop_back();
	for (int x = 0; x < void_userclass_pointers.back(); x++) {
		instance_referances.pop_back();
	}
	void_userclass_pointers.pop_back();
}

bool layer_cmp(const  Layer& lhs, const Layer& rhs)
{
	return lhs.z_index < rhs.z_index;
}
void Game::Sort() {
	// Sort layers
	std::sort(layers.begin(), layers.end(), layer_cmp);
	for (int x = 0; x < layers.size(); x++) {
		layers[x].chain.clear();
	}
	for (int x = 0; x < cp_rect_data.size(); x++) {
		for (int y = 0; y < layers.size(); y++) {
			if (cp_rect_data[x].layer_number == y + 1) {
				Graphic g;
				g.gtype = 0;
				g.content.rect = cp_rect_data[x];
				layers[y].chain.push_back(g);
			}
		}
	}
	for (int x = 0; x < cp_texture_data.size(); x++) {
		for (int y = 0; y < layers.size(); y++) {
			if (cp_texture_data[x].layer_number == y + 1) {
				Graphic g;
				g.gtype = 1;
				g.content.texture = cp_texture_data[x];
				layers[y].chain.push_back(g);
			}
		}
	}
}

int Game::LookUpVariable(const std::string &name) {
	if (name.find(".") != std::string::npos) {
		for (auto user_field = instances.begin(); user_field != instances.end(); ++user_field) {
			return user_field->LookupField(name);
		}
	}
	else {
		for (auto integer = integers.begin(); integer != integers.end(); ++integer) {
			if (integer->name == name) {
				return integer->value;
			}
		}
	}
}

void Game::setData(std::string command) {
	return;
}
std::string Game::formatDataString(std::string input) {
	return input;
}