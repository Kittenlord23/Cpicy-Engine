#include "Compiler.h"
#include "Editor.h"
#include <algorithm>
#include "Kiran.h"
#include "CpicyVector.h"

template<class T>
std::vector<T> GetSubVector(const std::vector<T>& vec, int start_index, int end_index)
{
	std::vector<T> r = {};
	for (int x = start_index; x < end_index; x++) {
		r.push_back(vec[x]);
	}
	return r;
}

void ImportFiles(std::vector<std::string>& lines) {
	std::vector<std::vector<std::string>> collection = {};
	for (auto line = lines.begin(); line != lines.end(); ++line) {
		if (line->find("import(") != std::string::npos) {
			std::string buffer;
			std::vector<std::string> target;
			std::ifstream openfile(GetExecutablePath() + "\\bootstrap\\" + replace(replace(*line, "import(", ""), ")", "") + ".source");
			while (std::getline(openfile, buffer)) {
				target.push_back(buffer);
			}
			collection.push_back(target);
			line->clear();
		}
	}
	for (auto left = collection.begin(); left != collection.end(); ++left) {
		lines.insert(lines.begin(), left->begin(), left->end());
	}
}

void ClearBlankLines(std::vector<std::string>& lines) {
	std::vector<int> positions = {};
	int index = 0;
	for (auto line = lines.begin(); line != lines.end(); ++line) {
		if (replace(*line, " ", "") == "") {
			positions.push_back(index);
		}
		index++;
	}
	std::vector<int> positions_after_shift = {};
	for (auto index : positions) {
		positions_after_shift.push_back(index);
		for (int x = 0; x < positions.size(); x++) {
			positions[x]--;
		}
	}
	for (auto index : positions_after_shift) {
		lines.erase(lines.begin() + index);
	}
}

void ClearCommentLines(std::vector<std::string>& lines, Game* game) {
	game->start_chain.chain.clear();
	std::vector<int> positions = {};
	int index = 0;
	for (auto line = lines.begin(); line != lines.end(); ++line) {
		if (replace(*line, " ", "").size() && replace(*line, " ", "").front() == '#') {
			positions.push_back(index);
		}
		index++;
	}
	std::vector<int> positions_after_shift = {};
	for (auto index : positions) {
		positions_after_shift.push_back(index);
		for (int x = 0; x < positions.size(); x++) {
			positions[x]--;
		}
	}
	for (auto index : positions_after_shift) {
		lines.erase(lines.begin() + index);
	}

	for (auto line = lines.begin(); line != lines.end(); ++line) {
		if (line->find("assign") != std::string::npos) {
			if (line->find("->") != std::string::npos) {
				// assign var->value syntax
				std::string name = replace(split(*line, "->").front(), "assign ", "");
				std::string value = split(*line, "->").back();
				*line = "set: " + name + "=" + value;
			}
			else {
				// assign var to value syntax
				std::string name = replace(split(*line, " to ").front(), "assign ", "");
				std::string value = split(*line, " to ").back();
				*line = "set: " + name + "=" + value;
			}
		}
		if (line->find("fetch") != std::string::npos) {
			*line = replace(*line, "fetch ", "get: ");
		}
	}

	for (auto line = lines.begin(); line != lines.end(); ++line) {
		if (replace(*line, TAB, "").size() != 0 && replace(*line, TAB, "").front() == '~') {
			Link l;
			l.action = replace(*line, TAB, "");
			l.action.erase(l.action.begin());
			l.trigger.cause = "pushed";
			game->start_chain.chain.push_back(l);
		}
	}

}

void CompressIfLines(std::vector<std::string>& lines) {
	int depth = 0;
	int line_number = 0;
	std::vector<int>start_ifs = {};
	std::vector<int> start_whiles = {};
	std::vector<int> end_whiles = {};
	std::vector<int> end_ifs = {};
	for (auto line = lines.begin(); line != lines.end(); ++line) {
		if (line->find("begin if") != std::string::npos) {
			depth++;
			if (depth == 1) {
				start_ifs.push_back(line_number);
			}
		}
		else if (line->find("end if") != std::string::npos) {
			depth--;
			if (depth == 0) {
				end_ifs.push_back(line_number);
			}
		}
		else if (line->find("begin while") != std::string::npos) {
			depth++;
			if (depth == 1) {
				start_whiles.push_back(line_number);
			}
		}
		else if (line->find("end while") != std::string::npos) {
			depth--;
			if (depth == 0) {
				end_whiles.push_back(line_number);
			}
		}
		line_number++;
	}

	start_ifs.insert(start_ifs.end(), start_whiles.begin(), start_whiles.end());
	end_ifs.insert(end_ifs.end(), end_whiles.begin(), end_whiles.end());
	std::vector<int> block_begins = start_ifs;
	std::vector<int> block_ends = end_ifs;
	enum SeperatorModeObject {
		if_splitter = false,
		while_splitter = true,
	};
	for (int x = 0; x < block_begins.size(); x++) {
		auto target = GetSubVector<std::string>(lines, block_begins[x], block_ends[x] + 1);
		int hash_depth = 99;
		std::string final_line = "";
		std::vector<SeperatorModeObject> unscope_log = {};
		for (auto line = target.begin(); line != target.end(); ++line) {
			if (line->find("begin if") != std::string::npos) {
				unscope_log.push_back(SeperatorModeObject::if_splitter);
				hash_depth--;
			}
			else if (line->find("begin while") != std::string::npos) {
				unscope_log.push_back(SeperatorModeObject::while_splitter);
				hash_depth--;
			}
			else if (line->find("end if") != std::string::npos) {
				unscope_log.pop_back();
				hash_depth++;
			}
			else if (line->find("end while") != std::string::npos) {
				unscope_log.pop_back();
				hash_depth++;
			}
			final_line += *line;
			if (unscope_log.size() && unscope_log.back() == SeperatorModeObject::if_splitter) {
				final_line += "%IF_SPLITTER" + std::to_string(hash_depth) + "%";
			}
			else if (unscope_log.size()) {
				final_line += "%WHILE_SPLITTER" + std::to_string(hash_depth) + "%";
			}
		}
		lines[block_begins[x]].clear();
		lines[block_begins[x]] = replace(final_line, TAB, "");
		for (int vanish = start_ifs[x] + 1; vanish < end_ifs[x] + 1; vanish++) {
			lines[vanish] = "";
		}
	}
}

void CompressWhileLines(std::vector<std::string>& lines) {
	int depth = 0;
	int line_number = 0;
	std::vector<int> start_whiles = {};
	std::vector<int> end_whiles = {};
	for (auto line = lines.begin(); line != lines.end(); ++line) {
		if (line->find("begin while") != std::string::npos && line->find("begin if") == std::string::npos) {
			depth++;
			if (depth == 1) {
				start_whiles.push_back(line_number);
			}
		}
		else if (line->find("end while") != std::string::npos && line->find("end if") == std::string::npos) {
			depth--;
			if (depth == 0) {
				end_whiles.push_back(line_number);
			}
		}
		line_number++;
	}
	for (auto index = 0; index < start_whiles.size(); index++) {
		auto sub_vec = GetSubVector<std::string>(lines, start_whiles[index], end_whiles[index] + 1);
		lines[start_whiles[index]].clear();
		for (int sub_ivec = 0; sub_ivec < sub_vec.size(); sub_ivec++) {
			if (sub_ivec != 0) {
				lines[start_whiles[index]].append("%WHILE_SPLITTER%");
			}
			lines[start_whiles[index]].append(replace(sub_vec[sub_ivec], TAB, ""));
		}
		for (int vanish = start_whiles[index] + 1; vanish < end_whiles[index] + 1; vanish++) {
			lines[vanish] = "";
		}
	}
}

int FindChainLines(std::vector<std::string>& lines, int& first, int& second, int& third, std::vector<CompileTimeErrorHandel>* errors) {
	int line_number = 0;
	bool found_event = false;
	bool found_loop = false;
	for (auto line = lines.begin(); line != lines.end(); ++line) {
		if (line->find("On Event:") != std::string::npos) {
			first = line_number;
			found_event = true;
		}
		if (line->find("On Loop:") != std::string::npos) {
			second = line_number;
			found_loop = true;
		}
		line_number++;
	}
	if (!found_event) {
		first = -1;
	}
	if (!found_loop) {
		second = -1;
	}
	third = lines.size();
	return 0;
}

void LoadEventLine(std::vector<std::string>& lines, Game* game, const int& second, const int& third, std::vector<CompileTimeErrorHandel>* errors, bool& compile_time_error) {
	game->cp_EChain_data.clear();
	if (second == -1) {
		return;
	}
	std::vector<std::string> event_lines = GetSubVector<std::string>(lines, second + 1, third);
	std::vector<std::vector<std::string>>EChains = {};
	auto t = GetIndexs<std::string>(event_lines, "When: ");
	for (int x = 0; x < t.size(); x++) {
		if (x + 1 < t.size()) {
			EChains.push_back(GetSubVector<std::string>(event_lines, t[x], t[x + 1]));
		}
		else {
			EChains.push_back(GetSubVector<std::string>(event_lines, t[x], event_lines.size()));
		}
	}
	for (int x = 0; x < EChains.size(); x++) {
		if (EChains[x].size() == 1) {
			continue;
		}
		Chain chain;
		Link scope_link;
		scope_link.action = "__scope__";
		scope_link.send_data = "none";
		scope_link.push = true;
		scope_link.trigger.cause = "none";
		chain.chain.push_back(scope_link);
		for (int y = 0; y < EChains[x].size(); y++) {
			try {
				Link link;
				if (y == 0) {
					link.push = true;
					link.send_data = "none";
					link.action = replace(replace(EChains[x][1], "Do: ", ""), TAB, "");
					link.trigger.cause = split(replace(replace(EChains[x][0], "When: ", ""), TAB, ""), "[").front();
					link.trigger.cause_value = game->GetCauseValueFromString(link.trigger.cause, replace(split(replace(replace(EChains[x][0], "When: ", ""), TAB, ""), "[").back(), "]", ""));
					chain.chain.push_back(link);
				}
				else if (y != 1) {
					link.push = true;
					link.send_data = "none";
					link.action = replace(replace(EChains[x][y], "Then: ", ""), TAB, "");
					link.trigger.cause = "pushed";
					chain.chain.push_back(link);
				}
			}
			catch (int e) {
				CompileTimeErrorHandel error;
				error.error_number = e;
				error.line_content = EChains[x][0];
				error.line_number = 0;
				errors->push_back(error);
				compile_time_error = true;
				MessageBoxA(NULL, "Failed to convert user code into runnable code", "Converter", MB_OK | MB_ICONERROR);
			}
		}
		scope_link.action = "__unscope__";
		scope_link.push = false;
		chain.chain.push_back(scope_link);
		game->cp_EChain_data.push_back(chain);
	}
	
}

void LoadLoopLine(std::vector<std::string>& lines, Game* game, const int& second, const int& third, std::vector<CompileTimeErrorHandel>* errors, bool& compile_time_error) {
	game->cp_LChain_data.clear();
	if (second == -1) {
		return;
	}
	std::vector<std::string> loop_lines = GetSubVector<std::string>(lines, second + 1, third);
	std::vector<std::vector<std::string>>LChains = {};
	auto t = GetIndexs<std::string>(loop_lines, "When: ");
	for (int x = 0; x < t.size(); x++) {
		if (x + 1 < t.size()) {
			LChains.push_back(GetSubVector<std::string>(loop_lines, t[x], t[x + 1]));
		}
		else {
			LChains.push_back(GetSubVector<std::string>(loop_lines, t[x], loop_lines.size()));
		}
	}
	for (int x = 0; x < LChains.size(); x++) {
		if (LChains[x].size() == 1) {
			continue;
		}
		Chain chain;
		for (int y = 0; y < LChains[x].size(); y++) {
			try {
				Link link;
				if (y == 0) {
					link.push = true;
					link.send_data = "none";
					link.action = replace(replace(LChains[x][1], "Do: ", ""), TAB, "");
					link.trigger.cause = split(replace(replace(LChains[x][0], "When: ", ""), TAB, ""), "[").front();
					link.trigger.cause_value = game->GetCauseValueFromString(link.trigger.cause, replace(split(replace(replace(LChains[x][0], "When: ", ""), TAB, ""), "[").back(), "]", ""));
					chain.chain.push_back(link);
				}
				else if (y != 1) {
					link.push = true;
					link.send_data = "pushed";
					link.action = replace(replace(LChains[x][y], "Then: ", ""), TAB, "");
					link.trigger.cause = "none";
					chain.chain.push_back(link);
				}
			}
			catch (int e) {
				CompileTimeErrorHandel error;
				error.error_number = e;
				error.line_content = LChains[x][0];
				error.line_number = 0;
				errors->push_back(error);
				compile_time_error = true;
			}
		}
		game->cp_LChain_data.push_back(chain);
	}
}

int LoadFunctionLine(std::vector<std::string>& lines, Game* game, std::vector<CompileTimeErrorHandel>* errors) {
	game->functions.clear();
	std::vector<int> begin_indexs = {};
	std::vector<int> end_indexs = {};
	{
		int index = 0;
		for (auto line = lines.begin(); line != lines.end(); ++line) {
			if (line->find("begin function") != std::string::npos) {
				begin_indexs.push_back(index);
			}
			if (line->find("end function") != std::string::npos) {
				end_indexs.push_back(index);
			}
			index++;
		}
	}
	if (begin_indexs.size() != end_indexs.size()) {
		CompileTimeErrorHandel h;
		h.error_number = 8;
		errors->push_back(h);
		return 1;
	}
	for (int f = 0; f < begin_indexs.size(); f++) {
		UserFunction function;
		function.arg_names = split(replace(replace(split(lines[begin_indexs[f]], "(").back(), ")", ""), " ", ""), ",");
		function.name = split(split(lines[begin_indexs[f]], "begin function ").back(), "(").front();
		auto commands = GetSubVector<std::string>(lines, begin_indexs[f] + 1, end_indexs[f]); 
		for (auto opper = commands.begin(); opper != commands.end(); ++opper) {
			*opper = replace(*opper, TAB, "");
		}
		function.commands = commands;
		game->functions.push_back(function);
	}
	return 0;
}
 
int LoadClassLine(std::vector<std::string>& lines, Game* game, std::vector<CompileTimeErrorHandel>* errors) {
	std::vector<int> begin_indexs = {};
	std::vector<int> end_indexs = {};
	{
		int index = 0;
		for (auto line = lines.begin(); line != lines.end(); ++line) {
			if (line->find("begin class") != std::string::npos) {
				begin_indexs.push_back(index);
			}
			if (line->find("end class") != std::string::npos) {
				end_indexs.push_back(index);
			}
			index++;
		}
	}
	if (begin_indexs.size() != end_indexs.size()) {
		std::string message = "Inbalance of 'begin class' and 'end class' lines (" + std::to_string(begin_indexs.size()) + " to " + std::to_string(end_indexs.size()) + ")";
		MessageBoxA(NULL, message.c_str(), "Error", MB_ICONERROR);
		return 1;
	}
	for (int x = 0; x < begin_indexs.size(); x++) {
		UserClass target = UserClass(replace(lines[begin_indexs[x]], "begin class ", ""));
		auto opper = GetSubVector<std::string>(lines, begin_indexs[x] + 1, end_indexs[x]);
		int fields_begin = -1;
		int fields_end = -1;
		std::vector<int> methods_begin = {};
		std::vector<int> methods_end = {};
		int line_number = 0;
		for (auto ol = opper.begin(); ol != opper.end(); ++ol) {
			if (replace(*ol, " ", "") == "beginfields") {
				fields_begin = line_number;
			}
			else if (ol->find("end fields") != std::string::npos) {
				fields_end = line_number;
			}
			else if (ol->find("begin method") != std::string::npos) {
				methods_begin.push_back(line_number);
			}
			 else if (ol->find("end method") != std::string::npos) {
				methods_end.push_back(line_number);
			}
			line_number++;
		}
		auto field_lines = GetSubVector<std::string>(opper, fields_begin, fields_end); // CompileTimeErrorHandel if fields_begin / end is uninitlized
		for (auto f = field_lines.begin(); f != field_lines.end(); ++f) {
			if (split(split(*f, "<").back(), ">").front() == "integer") {
				try {
					CpicyInteger field = CpicyInteger(replace(split(split(split(*f, "<").back(), "> ").back(), "=").front(), " ", ""), std::stoi(replace(split(split(split(*f, "<").back(), "> ").back(), "=").back(), " ", "")), FIELD_SCOPE);

					target.AddInteger("." + field.name, field.value);
				}
				catch (std::invalid_argument) {
					std::string L = "Line: " + *f + " failed to initalize static value";
					MessageBoxA(NULL, L.c_str(), "Error", MB_ICONERROR | MB_OK);
					return -1;
				}
			}
			else if (split(split(*f, "<").back(), ">").front() != "integer") {
				for (auto cl = game->classes.begin(); cl != game->classes.end(); ++cl) {
					if (cl->name == split(split(*f, "<").back(), ">").front()) {
						target.AddField(*cl, split(*f, "> ").back());
					}
				}
			}
		}
		for (auto indexer = 0; indexer < methods_begin.size(); indexer++) {
			UserFunction f;
			auto method_body = GetSubVector<std::string>(opper, methods_begin[indexer], methods_end[indexer]);
			if (method_body.size() != 0) {
				f.name = target.name + "~" +  replace(replace(split(method_body[0], "(").front(), "begin method", ""), " ", "");
				std::string temporary_arg_names = split(method_body[0], "(").back();
				temporary_arg_names.pop_back();

				f.arg_names = split(temporary_arg_names, ",");
				method_body.erase(method_body.begin());
				f.commands = method_body;
				game->functions.push_back(f);
				// f.commands.insert(method_body.begin() + 1, "__method_allocate_self__")
				// f.commands.push_back("assign 
			}
			
		}
		game->classes.push_back(target);
	}
	return 0;
}

int LoadStaticVariables(std::vector<std::string>& lines, Game* game, std::vector<CompileTimeErrorHandel>* errors) {
	for (auto line = lines.begin(); line != lines.end(); ++line) {
		if (line->find("static<") != std::string::npos) {
			if (split(split(*line, "<").back(), ">").front() == "integer") {
				CpicyInteger i = CpicyInteger(replace(split(split(*line, "> ").back(), "=").front(), " ", ""), std::stoi(replace(split(split(*line, "> ").back(), "=").back(), " ", "")), GLOBAL_SCOPE);
				game->integers.push_back(i);
			}



			else {
				for (auto cls = game->classes.begin(); cls != game->classes.end(); ++cls) {
					if (cls->name == split(split(*line, "<").back(), ">").front()) {
						 
						UserInstance i = UserInstance(cls->name, replace(split(*line, ">").back(), " ", ""), game->classes, GLOBAL_SCOPE);
						game->instances.push_back(i);
					}
				}
			}
		 }
	}
	return 0;
}
 
int compile(std::vector<std::string> input, Game* game, std::vector<CompileTimeErrorHandel>* errors) {
	errors->clear();
	game->instances.clear();
	game->instance_referances.clear();
	game->integer_pointers.clear();
	game->functions.clear();
	game->integers.clear();
	game->classes.clear();
	bool compile_time_error;
	int event_begin, loop_begin, lines_end;
	ImportFiles(input);
	ClearBlankLines(input);
	ClearCommentLines(input, game);
	CompressIfLines(input);
	ClearBlankLines(input);
	ClearBlankLines(input);
	if (FindChainLines(input, event_begin, loop_begin, lines_end, errors)) {
		return -1;
	}
	LoadEventLine(input, game, event_begin, loop_begin, errors, compile_time_error);
	LoadLoopLine(input, game, loop_begin, lines_end, errors, compile_time_error);
	if (LoadFunctionLine(input, game, errors)) {
		return -1;
	}
	if (LoadClassLine(input, game, errors) == -1) {
		return -1;
	}
	LoadStaticVariables(input, game, errors);
	for (auto line = input.begin(); line != input.end(); ++line) {
		std::cout << *line << std::endl;
	}
	return 0;
} 