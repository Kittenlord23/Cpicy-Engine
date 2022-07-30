#include "Chain.h"
#include "Game.h"
#include "RandomEngine.h"
#include "Clock.h"
#include "MathExpression.h"
#include <cassert>

struct DivideBy0 {
	static const int exception = 0;
};

CauseValue::CauseValue() {

}

void Chain::InterpretChain(SDL_Event e, Game* game, Clock c, SDL_Renderer* renderer) {
	// check to see if the chain is a valid size
	if (this->chain.size() != 0) {
		for (int x = 0; x < chain.size(); x++) {
			if (chain[x].trigger.cause == "start chain") {
				current_data = RunAction(&chain[x], "none", game, renderer, 98);
			}
			if (chain[x].trigger.cause == "key press") {
				if (e.type == SDL_KEYDOWN) {
					if (e.key.keysym.sym == chain[x].trigger.cause_value.key) {
						current_data = RunAction(&chain[x], "none", game, renderer, 98);
						if (!chain[x].push) {
							break;
						}
						continue;
					}
					else
						break;
				}
				else
					break;

			}
			if (chain[x].trigger.cause == "key release") {
				if (e.type == SDL_KEYUP) {
					if (e.key.keysym.sym == chain[x].trigger.cause_value.key) {
						current_data = RunAction(&chain[x], "none", game, renderer, 98);
						if (!chain[x].push) {
							break;
						}
						continue;
					}
					else
						break;
				}
				else
					break;
			}
			if (chain[x].trigger.cause == "time passed") {
				chain[x].trigger.cause_value.timer.current += 0.1 * c.deltaTime;
				if (chain[x].trigger.cause_value.timer.current > chain[x].trigger.cause_value.timer.dest) {
					chain[x].trigger.cause_value.timer.current = 0;
					chain[x].push = true;
					current_data = RunAction(&chain[x], current_data, game, renderer, 98);
				}
				else {
					chain[x].push = false;
					break;
				}
			}
			if (chain[x].trigger.cause == "in air") {
				if (game->game_camera.in_air) {
					chain[x].push = true;
					current_data = RunAction(&chain[x], current_data, game, renderer, 98);
				}
			}
			if (chain[x].trigger.cause == "pushed" || chain[x].trigger.cause == "none") {
				current_data = RunAction(&chain[x], current_data, game, renderer, 98);
				if (!chain[x].push) {
					break;
				}
				continue;
			}
		}
	}
}
std::string Chain::RunAction(Link* link, std::string incoming_data, Game* game, SDL_Renderer* renderer, int recersion_count) {
	// this was made before the discovery of switch statements...
	if (link->action.find("print: ") != std::string::npos && link->action.find("begin if") == std::string::npos && link->action.find("begin while") == std::string::npos) {
		std::string in = replace(replace(game->formatDataString(replace(replace(link->action, "%data%", incoming_data), "print: ", "")), "%newline%", "\n"), "%nl%", "\n");
		std::string out = "";
		auto stream = split(in, "<");
		for (auto part : stream) {
			if (split(part, ">").size() == 2) {
				std::string search = split(part, ">").front();
				for (auto var : game->integers) {
					if (var.name == search) {
						part = replace(replace(part, search, std::to_string(var.value)), ">", "");
						out += part;
					}
				}
			}
			else {
				out += part;
			}
		}
		std::cout << out;
		return  "none";
	}
	else if (link->action.find("move camera x to: ") != std::string::npos && link->action.find("begin if") == std::string::npos && link->action.find("begin while") == std::string::npos) {
		try {
			game->game_camera.x = EMath::evaluateMath(replace(replace(link->action, "move camera x to: ", ""), " ", ""), game);
			return std::to_string(game->game_camera.x);
		}
		catch (std::invalid_argument::exception) {
			link->push = false;
			game->logError(link->action, "A non numeric value was found", "move camera x to: NotANumber");
		}
	}
	else  if (link->action.find("move camera y to: ") != std::string::npos && link->action.find("begin if") == std::string::npos && link->action.find("begin while") == std::string::npos) {
		try {
			game->game_camera.y = EMath::evaluateMath(replace(replace(link->action, "move camera x to: ", ""), " ", ""), game);
			return std::to_string(game->game_camera.y);
		}
		catch (std::invalid_argument::exception) {
			link->push = false;
			game->logError(link->action, "A non numeric value was found", "move camrea y to: NotANumber");
		}
	}
	else if (link->action.size() > 10 &&  link->action.substr(0, 11) == "begin while") {
		auto opper = split(link->action, "%WHILE_SPLITTER" + std::to_string(recersion_count) + "%");
		std::string condition = replace(replace(split(opper[0], "begin while").back(), "(", ""), ")", "");
		opper.erase(opper.begin());
		Chain c;
		for (auto link = opper.begin(); link != opper.end(); ++link) {
			Link l;
			l.action = *link;
			l.push = true;
			l.send_data = "none";
			l.trigger.cause = "push";
			c.chain.push_back(l);
		}
		while (evalStrAsBool(condition, game)) {
			game->scope();
			for (auto link = c.chain.begin(); link != c.chain.end(); ++link) {
				incoming_data = RunAction(&(*link), incoming_data, game, renderer, recersion_count - 1);
			}
			game->unscope();
		}
	}
	else  if (link->action.size() > 7 && link->action.substr(0, 8) == "begin if") {
		std::vector<std::string> opper = split(link->action, "%IF_SPLITTER" + std::to_string(recersion_count) + "%");
		std::vector<int> end_if_indexes = GetIndexs<std::string>(opper, "end if");
		std::reverse(end_if_indexes.begin(), end_if_indexes.end());
		int end_if_jump = 0;
		for (int x = 0; x < opper.size(); x++) {
			if (opper[x].find("begin if") != std::string::npos) {
				if (evalStrAsBool(replace(replace(replace(replace(opper[x], "begin if", " "), "(", ""), ")", ""), " ", ""), game)) {
					end_if_jump++;
					continue;
				}
				else {
					x = end_if_indexes[end_if_jump];
					end_if_jump++;
				}
			}
			else {
				Link l;
				l.action = opper[x];
				l.push = true;
				l.send_data = "none";
				incoming_data = RunAction(&l, incoming_data, game, renderer, recersion_count - 1);
			}
		}
	}
	if (link->action.find("swap image: ") != std::string::npos && link->action.find("begin if") == std::string::npos && link->action.find("begin while") == std::string::npos) {
		auto args = split(link->action, ":");
		args.erase(args.begin());
		if (args.size() != 2) {
			game->logError(link->action, "Requires 2 arguments", "swap image: id:new");
			return "none";
		}
		for (int x = 0; x < game->cp_texture_data.size(); x++) {
			for (int z = 0; z < game->loaded_textures.size(); z++) {
				if (args[1] == game->loaded_textures[z].name) {
					game->cp_texture_data[x].texture.name = game->loaded_textures[z].name;
					game->Sort();
					break;
				}
			}
		}
	}
	if (link->action.find("get: ") != std::string::npos && link->action.find("begin if") == std::string::npos && link->action.find("begin while") == std::string::npos) {
		for (int x = 0; x < game->integer_pointers.size(); x++) {
			if (game->integer_pointers[x].name == replace(link->action, "get: ", "") && (game->integer_pointers[x].scope_level == game->function_call_stack_count || game->integer_pointers[x].scope_level == GLOBAL_SCOPE)) {
				return std::to_string(*game->integer_pointers[x].modify);
			}
		}
		for (int x = game->instances.size() - 1; x >= 0; x--) {
			for (int y = 0; y < game->instances[x].fields.size(); y++) {
				if (game->instances[x].identifier + replace(game->instances[x].fields[y].identifier, game->instances[x].fields[y].identifier + ".", "") == split(link->action, ": ").back() && (game->instances[x].scope_level == GLOBAL_SCOPE || game->instances[x].scope_level == game->function_call_stack_count)) {
					return std::to_string(game->instances[x].fields[y].value);
				}
			}
		}
		for (int x = game->integers.size() - 1; x >= 0; x--) {
			if (strcmp(game->integers[x].name.c_str(), replace(link->action, "get: ", "").c_str()) && (game->integers[x].scope_level == game->function_call_stack_count || game->integers[x].scope_level == GLOBAL_SCOPE)) {
				return std::to_string(game->integers[x].value);
			}
		}
		for (int x = game->instance_referances.size() - 1; x >= 0; x--) {
			for (int y = 0; y < game->instance_referances[x].ptr->fields.size(); y++) {
				if (game->instance_referances[x].identifier + game->instance_referances[x].ptr->fields[y].identifier == split(link->action, ": ").back()) {
					return std::to_string(game->instance_referances[x].ptr->fields[y].value);
				}
			}
		}
		link->push = false;
		game->logError(link->action, "Could not find any variable under the given name or in this scope", "get: undefined");
	}
	else if (link->action.find("__scope__") != std::string::npos && link->action.find("begin if") == std::string::npos && link->action.find("begin while") == std::string::npos) {
		game->scope();
	}
	else if (link->action.find("__unscope__") != std::string::npos && link->action.find("begin if") == std::string::npos && link->action.find("begin while") == std::string::npos) {
		game->unscope();
	}
	else if (link->action.find("set gravity force:") != std::string::npos && link->action.find("begin if") == std::string::npos && link->action.find("begin while") == std::string::npos) {
		game->gravity_force = std::stoi(replace(link->action, "set gravity force:", ""));
	}
	else if (link->action.find("new<") != std::string::npos && link->action.find("begin if") == std::string::npos && link->action.find("begin while") == std::string::npos) {
		std::string type = split(split(link->action, "<").back(), ">").front();
		if (type == "integer") {
			game->void_integers.back()++;
			game->integers.push_back(CpicyInteger(replace(split(split(link->action, "=").front(), ">").back(), " ", ""), EMath::evaluateMath(replace(split(link->action, "=").back(), " ", ""), game), game->function_call_stack_count));
		}
		else {
			for (auto cl = game->classes.begin(); cl != game->classes.end(); ++cl) {
				if (cl->name == type) {
					UserInstance instance = UserInstance(type, replace(split(link->action, ">").back(), " ", ""), game->classes, game->function_call_stack_count);
					game->void_userclasses.back()++;
					game->instances.push_back(instance);
				}
			}
		}
	}
	else if (link->action.find("set: ") != std::string::npos && link->action.find("begin if") == std::string::npos && link->action.find("begin while") == std::string::npos) {
		for (int x = game->integers.size() - 1; x >= 0; x--) {
			auto _T = replace(game->formatDataString(link->action), "%data%", incoming_data);
			if (replace(game->integers[x].name, "<integer>", "") == replace(split(replace(_T, "set: ", ""), "=")[0], " ", "") && (game->integers[x].scope_level == GLOBAL_SCOPE || game->integers[x].scope_level == game->function_call_stack_count)) {
				game->integers[x].value = EMath::evaluateMath(split(replace(_T, "set: ", ""), "=")[1], game);
				return std::to_string(game->integers[x].value);
			}
		}
		for (int x = 0; x < game->integer_pointers.size(); x++) {
			auto _T = replace(game->formatDataString(link->action), "%data%", incoming_data);
			if (replace(game->integer_pointers[x].name, "<&integer>", "") == replace(split(replace(_T, "set: ", ""), "=")[0], " ", "") && (game->integer_pointers[x].scope_level = game->function_call_stack_count || game->integers[x].scope_level == GLOBAL_SCOPE)) {
				*game->integer_pointers[x].modify = EMath::evaluateMath(split(replace(_T, "set: ", ""), "=")[1], game);
				return std::to_string(*game->integer_pointers[x].modify);
			}
		}
		for (int x = 0; x < game->instances.size(); x++) {
			auto _T = replace(game->formatDataString(link->action), "%data%", incoming_data);
			for (int y = 0; y < game->instances[x].fields.size(); y++) {
				if (replace(game->instances[x].identifier + game->instances[x].fields[y].identifier, " ", "") == replace(replace(split(_T, "=").front(), "set: ", ""), " ", "") && (game->instances[x].scope_level = game->function_call_stack_count || game->instances[x].scope_level == GLOBAL_SCOPE)) {

					EMath::evaluateMath(split(replace(_T, "set: ", ""), "=").front(), game);
					
					//game->instances[x].fields[y].value = EMath::evaluateMath(split(replace(_T, "set: ", ""), "=")[1], game);
					return std::to_string(game->instances[x].fields[y].value);
				}
			}
		}
		for (int x = 0; x < game->instance_referances.size(); x++) {
			auto _T = replace(game->formatDataString(link->action), "%data%", incoming_data);
			for (int y = 0; y < game->instance_referances[x].ptr->fields.size(); y++) {
				if (replace(game->instance_referances[x].identifier + game->instance_referances[x].ptr->fields[y].identifier, " ", "") == replace(replace(split(_T, "=").front(), "set: ", ""), " ", "")) {
					game->instance_referances[x].ptr->fields[y].value = EMath::evaluateMath(split(replace(_T, "set: ", "	 "), "=")[1], game);
					return std::to_string(game->instance_referances[x].ptr->fields[y].value);
				}
			}
		}
		// TODO: add abilty to set field values
		game->logError(link->action, "Could not find any variable under the given name", "set: undefined");
	}
	else  if (link->action.find("start right camera") != std::string::npos && link->action.find("begin if") == std::string::npos && link->action.find("begin while") == std::string::npos) {
		game->game_camera.right = true;
	}
	else if (link->action.find("start left camera") != std::string::npos && link->action.find("begin if") == std::string::npos && link->action.find("begin while") == std::string::npos) {
		game->game_camera.left = true;
	}
	else if (link->action.find("start up camera") != std::string::npos && link->action.find("begin if") == std::string::npos && link->action.find("begin while") == std::string::npos) {
		game->game_camera.up = true;
	}
	else  if (link->action.find("start down camera") != std::string::npos && link->action.find("begin if") == std::string::npos && link->action.find("begin while") == std::string::npos) {
		game->game_camera.down = true;
	}
	else  if (link->action.find("stop right camera") != std::string::npos) {
		game->game_camera.right = false;
	}
	else if (link->action.find("stop left camera") != std::string::npos && link->action.find("begin if") == std::string::npos && link->action.find("begin while") == std::string::npos) {
		game->game_camera.left = false;
	}
	else  if (link->action.find("stop down camera") != std::string::npos && link->action.find("begin if") == std::string::npos && link->action.find("begin while") == std::string::npos) {
		game->game_camera.down = false;
	}
	else if (link->action.find("set camera speed: ") != std::string::npos && link->action.find("begin if") == std::string::npos && link->action.find("begin while") == std::string::npos) {
		try {
			game->camera_speed = EMath::evaluateMath(game->formatDataString(replace(replace(link->action, "set camera speed: ", ""), "%data%", incoming_data)), game);
			return replace(link->action, "set camera speed: ", "");
		}
		catch (std::invalid_argument::exception) {
			game->logError(link->action, "A non numeric value was found", "set camera speed: NotANumber");
		}
	}
	else if (link->action.find("stop up camera") != std::string::npos && link->action.find("begin if") == std::string::npos && link->action.find("begin while") == std::string::npos) {
		game->game_camera.up = false;
	}
	else if (link->action.find("get camera speed") != std::string::npos && link->action.find("begin if") == std::string::npos && link->action.find("begin while") == std::string::npos) {
		return std::to_string(game->camera_speed);
	}
	else if (link->action.find("set rectangle draw color:") != std::string::npos && link->action.find("begin if") == std::string::npos && link->action.find("begin while") == std::string::npos) {
		auto args = split(link->action, ":");
		args.erase(args.begin());
		if (args.size() != 3) {
			game->logError(link->action, "Requires 3 arguments", "set rectangle draw color: r:g:b");
		}
		else {
			game->user_code_draw_color.r = EMath::evaluateMath(args[0], game);
			game->user_code_draw_color.g = EMath::evaluateMath(args[1], game);
			game->user_code_draw_color.b = EMath::evaluateMath(args[2], game);
		}
	}
	else if (link->action.find("add rectangle: ") != std::string::npos && link->action.find("begin if") == std::string::npos && link->action.find("begin while") == std::string::npos) {
		auto args = split(link->action, ":");
		args.erase(args.begin());
		if (args.size() != 7) {
			game->logError(link->action, "Requires 7 arguments", "add rectangle: x:y:w:h:move_with_camera:layer_number:id");
			return "none";
		}
		else {
			try {
				GRect add;
				add.x = EMath::evaluateMath(args[0], game);
				add.y = EMath::evaluateMath(args[1], game);
				add.w = EMath::evaluateMath(args[2], game);
				add.h = EMath::evaluateMath(args[3], game);
				if (args[4] == "true" || args[4] == "1") {
					add.dynamic_static = true;
				}
				else {
					add.dynamic_static = false;
				}
				add.layer_number = EMath::evaluateMath(args[5], game);
				add.id = args[6];
				add.r = game->user_code_draw_color.r;
				add.g = game->user_code_draw_color.g;
				add.b = game->user_code_draw_color.b;
				game->layers[add.layer_number].AddGraphic(add);
			}
			catch (std::invalid_argument) {
				game->logError(link->action, "Invalid argument at a position", "add rectangle: NotANumber");
			}
		}
	}
	else if (link->action.find("enable gravity") != std::string::npos && link->action.find("begin if") == std::string::npos && link->action.find("begin while") == std::string::npos) {
		game->enable_gravity = true;
	}
	else if (link->action.find("disable gravity") != std::string::npos && link->action.find("begin if") == std::string::npos && link->action.find("begin while") == std::string::npos) {
		game->enable_gravity = false;
	}
	else if (link->action.find("enable errors") != std::string::npos && link->action.find("begin if") == std::string::npos && link->action.find("begin while") == std::string::npos) {
		game->enable_errors = true;
	}
	else if (link->action.find("set data") != std::string::npos && link->action.find("begin if") == std::string::npos && link->action.find("begin while") == std::string::npos) {
		game->setData(replace(game->formatDataString(link->action), "%data%", incoming_data));
	}
	else if (link->action.find("random number: ") != std::string::npos && link->action.find("begin if") == std::string::npos && link->action.find("begin while") == std::string::npos) {
		std::string args[2];
		args[0] = game->formatDataString(split(replace(replace(link->action, "random number: ", ""), "%data%", incoming_data), ":").front());
		args[1] = game->formatDataString(split(replace(link->action, "random number: ", ""), ":").back());
		RandomInt output;
		output.Generate(std::stoi(args[0]), std::stoi(args[1]));

		return std::to_string(output.Result());
	}
	else if (link->action.find("move rectangle: ") != std::string::npos && link->action.find("begin if") == std::string::npos && link->action.find("begin while") == std::string::npos) {
		auto sp = split(replace(game->formatDataString(replace(link->action, "move rectangle: ", "")), "%data%", incoming_data), ":");
		for (auto rect = game->cp_rect_data.begin(); rect != game->cp_rect_data.end(); ++rect) {
			if (rect->id == sp[0]) {
				rect->x += std::stoi(sp[1]);
				rect->y += std::stoi(sp[2]);
				return "none";
			}
		}
	}
	else if (link->action.find("get rectangle value: ") != std::string::npos && link->action.find("begin if") == std::string::npos && link->action.find("begin while") == std::string::npos) {
		auto sp = split(replace(game->formatDataString(replace(link->action, "get rectangle value: ", "")), "%data%", incoming_data), ":");
		for (auto rect = game->cp_rect_data.begin(); rect != game->cp_rect_data.end(); ++rect) {
			if (rect->id == sp[0]) {
				if (sp[1] == "x" || sp[1] == "0") {
					return std::to_string(rect->x);
				}
				if (sp[1] == "y" || sp[1] == "1") {
					return std::to_string(rect->x);
				}
				if (sp[1] == "w" || sp[1] == "2") {
					return std::to_string(rect->x);
				}
				if (sp[1] == "h" || sp[1] == "3") {
					return std::to_string(rect->x);
				}
			}
		}
	}
	else if (link->action.find("do rectangles overlap: ") != std::string::npos && link->action.find("begin if") == std::string::npos && link->action.find("begin while") == std::string::npos) {
		auto sp = split(replace(game->formatDataString(replace(link->action, "do rectangles overlap: ", "")), "%data%", incoming_data), ":");
		GRect r1;
		GRect r2;
		for (int x = 0; x < game->cp_rect_data.size(); x++) {
			if (game->cp_rect_data[x].id == sp[0]) {
				r1 = game->cp_rect_data[x];
			}
			if (game->cp_rect_data[x].id == sp[1]) {
				r2 = game->cp_rect_data[x];
			}
		}
		if (r2.dynamic_static) {
			r2.x += game->game_camera.x;
			r2.y += game->game_camera.y;
		}
		if (r1.dynamic_static) {
			r1.x += game->game_camera.x;
			r1.y += game->game_camera.y;
		}
		bool overlap = false;
		if (r1.x > r2.x && r1.x < r2.x + r2.w) {
			if (r1.y > r2.y && r1.y < r2.y + r2.h) {
				overlap = true;
			}
		}
		if (r1.x + r1.w > r2.x && r1.x + r1.w < r2.x + r2.w) {
			if (r1.y + r1.h > r2.y && r1.y + r1.h < r2.y + r2.h) {
				overlap = true;
			}
		}
		if (overlap) {
			return "1";
		}
		else {
			return "0";
		}
	}
	else if (link->action.find("add: ") != std::string::npos || link->action.find("addition: ") != std::string::npos && link->action.find("begin if") == std::string::npos && link->action.find("begin while") == std::string::npos) {
		try {
			std::string args[2];
			args[0] = game->formatDataString(split(replace(replace(replace(link->action, "add: ", ""), "addition: ", ""), "%data%", incoming_data), ":").front());
			args[1] = game->formatDataString(split(replace(replace(replace(link->action, "add: ", ""), "addition: ", ""), "%data%", incoming_data), ":").back());
			return std::to_string(std::stod(args[0]) + std::stod(args[1]));
		}
		catch (std::invalid_argument::exception) {
			game->logError(link->action, "A non numeric value was found", "add: NotANumber:3");
		}
	}
	else if (link->action.find("sub: ") != std::string::npos || link->action.find("subtract: ") != std::string::npos && link->action.find("begin if") == std::string::npos && link->action.find("begin while") == std::string::npos) {
		try {
			std::string args[2];
			args[0] = game->formatDataString(split(replace(replace(replace(link->action, "sub: ", ""), "subtract: ", ""), "%data%", incoming_data), ":").front());
			args[1] = game->formatDataString(split(replace(replace(replace(link->action, "sub: ", ""), "subtract: ", ""), "%data%", incoming_data), ":").back());
			return std::to_string(std::stod(args[0]) - std::stod(args[1]));
		}
		catch (std::invalid_argument::exception) {
			game->logError(link->action, "A non numeric value was found", "sub: NotANumber:3");
		}
	}
	else if (link->action.find("multiply: ") != std::string::npos && link->action.find("begin if") == std::string::npos && link->action.find("begin while") == std::string::npos) {
		try {
			std::string args[2];
			args[0] = game->formatDataString(split(replace(replace(link->action, "multiply:", ""), " % data % ", incoming_data), " : ").front());
			args[1] = game->formatDataString(split(replace(replace(link->action, "multiply:", ""), "%data%", incoming_data), ":").back());
			return std::to_string(std::stod(args[0]) * std::stod(args[1]));
		}
		catch (std::invalid_argument::exception) {
			game->logError(link->action, "A non numeric value was found", "multiply: NotANumber:3");
		}
	}
	else if (link->action.find("divide: ") != std::string::npos && link->action.find("begin if") == std::string::npos && link->action.find("begin while") == std::string::npos) {
		try {
			std::string args[2];
			args[0] = game->formatDataString(split(replace(replace(link->action, "divide: ", ""), "%data%", incoming_data), ":").front());
			args[1] = game->formatDataString(split(replace(replace(link->action, "divide: ", ""), "%data%", incoming_data), ":").back());
			if (std::stod(args[1]) == double(0) || std::stod(args[1]) == double(-0)) {
				throw - 1;
			}
			return std::to_string(std::stod(args[0]) / std::stod(args[1]));
		}
		catch (std::invalid_argument::exception) {
			game->logError(link->action, "A non numeric value was found", "divide: NotANumber:3");
		}
		catch (int e) {
			if (e == -1) {
				game->logError(link->action, "Divide by 0", "divide: 3/0");
			}
		}
	}
	else if (link->action.find("sin: ") != std::string::npos && link->action.find("begin if") == std::string::npos && link->action.find("begin while") == std::string::npos) {
		try {
			std::string args[1];
			args[0] = game->formatDataString(split(replace(replace(link->action, "sin: ", ""), "%data%", incoming_data), ":").front());
			return std::to_string(sin(game->to_degrees(std::stod(args[0])))); // See implmentation of to_degrees
		}
		catch (std::invalid_argument::exception) {
			game->logError(link->action, "A non numeric value was found", "sin: NotANumber");
		}
	}
	else if (link->action.find("cos: ") != std::string::npos && link->action.find("begin if") == std::string::npos && link->action.find("begin while") == std::string::npos) {
		try {
			std::string args[1];
			args[0] = game->formatDataString(split(replace(replace(link->action, "cos: ", ""), "%data%", incoming_data), ":").front());
			return std::to_string(cos(game->to_degrees(std::stod(args[0]))));
		}
		catch (std::invalid_argument::exception) {
			game->logError(link->action, "A non numeric value was found", "cos: NotANumber");
		}
	}
	else if (link->action.find("tan: ") != std::string::npos && link->action.find("begin if") == std::string::npos && link->action.find("begin while") == std::string::npos) {
		try {
			std::string args[1];
			args[0] = game->formatDataString(split(replace(replace(link->action, "tan: ", ""), "%data%", incoming_data), ":").front());
			return std::to_string(tan(game->to_degrees(std::stod(args[0]))));
		}
		catch (std::invalid_argument::exception) {
			game->logError(link->action, "A non numeric value was found", "tan: NotANumber");
		}
	}
	else if (link->action.find("__degrees__") != std::string::npos && link->action.find("begin if") == std::string::npos && link->action.find("begin while") == std::string::npos) {
		game->using_degrees = true;
	}
	else if (link->action.find("__radians__") != std::string::npos && link->action.find("begin if") == std::string::npos && link->action.find("begin while") == std::string::npos) {
		game->using_degrees = false;
	}
	else {
		std::string function_check_name = split(link->action, ":").front();
		for (auto function_name = game->functions.begin(); function_name != game->functions.end(); ++function_name) {
			if (function_check_name == function_name->name) {
				game->function_call_stack_count++;
				game->scope();
				int arg_opper_index = 0;
				std::vector<std::string> arg_inputs = split(link->action, ":");
				arg_inputs.erase(arg_inputs.begin());
				for (auto arg = function_name->arg_names.begin(); arg != function_name->arg_names.end(); ++arg) {
					auto type = replace(split(*arg, ">").front(), "<", "");
					if (type == "integer") {
						game->integers.push_back(CpicyInteger(replace(split(function_name->arg_names[arg_opper_index], ">").back(), " ", ""), EMath::evaluateMath(arg_inputs[arg_opper_index], game), game->function_call_stack_count));
						game->void_integers.back()++;
					}
					else if (type == "&integer") {
						for (auto integer_name = game->integers.begin(); integer_name != game->integers.end(); ++integer_name) {
							if (replace(integer_name->name, " ", "") == replace(arg_inputs[arg_opper_index], " ", "") && (integer_name->scope_level == game->function_call_stack_count - 1 || integer_name->scope_level == GLOBAL_SCOPE)) {
								game->integer_pointers.push_back(CpicyIntegerRef(replace(split(arg[arg_opper_index], ">").back(), " ", ""), &integer_name->value, game->function_call_stack_count));
								game->integer_pointers.back().name = split(game->integer_pointers.back().name, ">").back();
								game->void_integer_pointers.back()++;
							}
						}
					}
					else {
						for (auto cl = game->classes.begin(); cl != game->classes.end(); ++cl) {
							if (cl->name == replace(type, "&", "")) {
								if (type.front() != '&') {
									for (auto name = game->instances.begin(); name != game->instances.end(); ++name) {
										if (replace(name->identifier, " ", "") == replace(arg_inputs[arg_opper_index], " ", "")) {
											UserInstance copy = UserInstance(*name);
											copy.identifier = replace(split(arg[arg_opper_index], ">").back(), " ", "");
											game->instances.push_back(copy);
											game->void_userclasses.back()++;
											goto class_search_end;
										}
									}
								}
								else {
									for (auto name = game->instances.begin(); name != game->instances.end(); ++name) {
										if (replace(name->identifier, " ", "") == replace(arg_inputs[arg_opper_index], " ", "")) {
											UserInstanceRef ref;
											ref.identifier = replace(split(arg[arg_opper_index], ">").back(), " ", "");
											ref.ptr = &(*name); // defref then ref it
											game->instance_referances.push_back(ref);
											game->void_userclass_pointers.back()++;
										}
									}
								}
							}
						}
					class_search_end:
						;
					}
					arg_opper_index++;
				}
				Chain function_chain;
				std::string data;
				for (int y = 0; y < function_name->commands.size(); y++) {
					Link l;
					l.push = true;
					l.trigger.cause = "pushed";
					l.send_data = "none";
					l.action = function_name->commands[y];
					data = this->RunAction(&l, data, game, renderer, 99);
					if (l.push == false) {
						link->push = false;
						break;
					}
				}
				game->unscope();
				game->function_call_stack_count--;
			}
		}
	}
	return "none";
}