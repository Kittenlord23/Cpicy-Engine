#include "Kiran.h"
#include "Game.h"
#include <direct.h>

int GetDesktopResolution(char axis)
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	if (axis == 'x')
		return desktop.right;
	else
		return desktop.bottom;
}
bool MakeDir(const char* path) {
	int sucsess = _mkdir(path);
	switch (sucsess) {
	case 0:
		return true;
	case -1:
		return false;
	default:
		return false;
	}
}
std::string GetFilePath(const TCHAR* selection) { 
	auto openFileDialog = [&](TCHAR szFileName[])
	{
		OPENFILENAME ofn;
		DWORD error_value;
		const TCHAR* FilterSpec = selection;
		const TCHAR* Title = "Open";

		const TCHAR* myDir = "Projects";

		TCHAR szFileTitle[MAX_PATH] = { '\0' };

		ZeroMemory(&ofn, sizeof(OPENFILENAME));

		*szFileName = 0;

		/* fill in non-variant fields of OPENFILENAME struct. */
		ofn.lStructSize = sizeof(OPENFILENAME);

		ofn.hwndOwner = GetFocus();
		ofn.lpstrFilter = FilterSpec;
		ofn.lpstrCustomFilter = NULL;
		ofn.nMaxCustFilter = 0;
		ofn.nFilterIndex = 0;
		ofn.lpstrFile = szFileName;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrInitialDir = myDir; // Initial directory.
		ofn.lpstrFileTitle = szFileTitle;
		ofn.nMaxFileTitle = MAX_PATH;
		ofn.lpstrTitle = Title;
		//ofn.lpstrDefExt = 0; // I've set to null for demonstration
		ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

		// false if failed or cancelled
		if (GetOpenFileName(&ofn) == 1)
		{
			return 1;
		}
		else
		{
			error_value = CommDlgExtendedError();
			return 0;
		}
	};
	TCHAR openedFileName[MAX_PATH];

	if (openFileDialog(openedFileName))
		return (std::string)openedFileName;
	else
		return (std::string)"canceld";
}
std::vector<std::string> split(std::string str, const std::string& token) {
	std::vector<std::string>result;
	while (str.size()) {
		int index = str.find(token);
		if (index != std::string::npos) {
			result.push_back(str.substr(0, index));
			str = str.substr(index + token.size());
			if (str.size() == 0)result.push_back(str);
		}
		else {
			result.push_back(str);
			str = "";
		}
	}
	return result;
}
std::string replace(std::string subject, const std::string& search, const std::string& replace) {
	size_t pos = 0;
	if (search == "") {
		return subject;
	}
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}
std::string GetExecutablePath(std::string exe_name) {
	char ownPth[MAX_PATH];
	HMODULE hModule = GetModuleHandle(NULL);
	if (hModule != NULL)
	{
		// Use GetModuleFileName() with module handle to get the path
		GetModuleFileName(hModule, ownPth, (sizeof(ownPth)));
		return replace(ownPth, exe_name, "");
	}
	return "failed";
}
std::vector<std::string> GetFiles(std::string folder)
{
	std::vector<std::string> names;
	std::string search_path = folder + "/*.*";
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				names.push_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}
bool is_digits(const std::string& str)
{
	return str.find_first_not_of("-0123456789") == std::string::npos;
}
bool evalStrAsBool(std::string str, Game* game) {
	str = replace(str, " ", "");
	for (auto instance = game->instances.begin(); instance != game->instances.end(); ++instance) {
		for (auto field = instance->fields.begin(); field != instance->fields.end(); ++field) {
			str = replace(str, instance->identifier + "." + field->identifier, std::to_string(field->value));
		}
	}
	for (auto var : game->integers) {
		str = replace(replace(str, "<integer>", ""), var.name, std::to_string(var.value));
	}
	for (auto var : game->integer_pointers) {
		str = replace(str, replace(var.name, "<&integer>", ""), std::to_string(*var.modify));
	}
	if (str.find("==") != std::string::npos) {
		std::vector<std::string> arguments = split(str, "==");
		if (is_digits(arguments[0]) && is_digits(arguments[1])) {
			if (std::stoi(arguments[0]) == std::stoi(arguments[1])) {
				return true;
			}
			else {
				return false;
			}
		}
		else if ((is_digits(arguments[0]) && !is_digits(arguments[1])) || (!is_digits(arguments[0]) && is_digits(arguments[1]))) {
			throw "bad comparison";
		}
		else {
			if (arguments[0] == arguments[1]) {
				return true;
			}
			else {
				return false;
			}
		}
	}
	else if (str.find("!=") != std::string::npos) {
		std::vector<std::string> arguments = split(str, "!=");
		if (is_digits(arguments[0]) && is_digits(arguments[1])) {
			if (std::stoi(arguments[0]) != std::stoi(arguments[1])) {
				return true;
			}
			else {
				return false;
			}
		}
		else if ((is_digits(arguments[0]) && !is_digits(arguments[1])) || (!is_digits(arguments[0]) && is_digits(arguments[1]))) {
			throw "bad comparison";
		}
		else {
			if (arguments[0] == arguments[1]) {
				return true;
			}
			else {
				return false;
			}
		}
	}
	else if (str.find(">") != std::string::npos) {
		std::vector<std::string> arguments = split(str, ">");
		if (is_digits(arguments[0]) && is_digits(arguments[1])) {
			if (std::stoi(arguments[0]) > std::stoi(arguments[1])) {
				return true;
			}
			else {
				return false;
			}
		}
		else
			throw "bad comparison";
	}
	else if (str.find("<") != std::string::npos) {
		std::vector<std::string> arguments = split(str, "<");
		if (is_digits(arguments[0]) && is_digits(arguments[1])) {
			if (std::stoi(arguments[0]) < std::stoi(arguments[1])) {
				return true;
			}
			else {
				return false;
			}
		}
		else
			throw "bad comparison";
	}
	else if (str.find(">=") != std::string::npos) {
		std::vector<std::string> arguments = split(str, ">=");
		if (is_digits(arguments[0]) && is_digits(arguments[1])) {
			if (std::stoi(arguments[0]) >= std::stoi(arguments[1])) {
				return true;
			}
			else {
				return false;
			}
		}
		else
			throw "bad comparison";
	}
	else if (str.find("<=") != std::string::npos) {
		std::vector<std::string> arguments = split(str, "<=");
		if (is_digits(arguments[0]) && is_digits(arguments[1])) {
			if (std::stoi(arguments[0]) <= std::stoi(arguments[1])) {
				return true;
			}
			else {
				return false;
			}
		}
		else
			throw "bad comparison";
	}
}
std::string GetClipboardData() {
	HANDLE h;

	if (!OpenClipboard(NULL))
		throw("Can't open clipboard");

	h = GetClipboardData(CF_TEXT);

	auto data = (char*)h;
	CloseClipboard();
	return std::string(data);
}
int find_last_index(std::string input, std::string search) {
	if (input.find(search) == std::string::npos) {
		return -1;
	}
	for (int x = input.size() - search.size(); x >= 0; x++) {
		std::string check = input.substr(x, x + search.size());
		if (check == search) {
			return x;
		}
	}
}