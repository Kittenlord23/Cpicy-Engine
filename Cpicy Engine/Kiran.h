#pragma once
#include <vector>
#include <Windows.h>
#include <iostream>
#include <string>

class Game;

int GetDesktopResolution(char axis);
bool MakeDir(const char* path);
std::string GetFilePath(const TCHAR* selection = "Spice project\0*.spiceP\0");
std::vector<std::string> split(std::string str, const std::string& token);
std::string replace(std::string subject, const std::string& search, const std::string& replace);
std::string GetExecutablePath(std::string exe_name = "Cpicy Engine.exe");
std::vector<std::string> GetFiles(std::string folder);
bool is_digits(const std::string& str);
bool evalStrAsBool(std::string str, Game* game);
std::string GetClipboardData();
int find_last_index(std::string input, std::string search);

template<class T>
inline const std::vector<int> GetIndexs(const std::vector<T>& vec, T search)
{
	std::vector<int> r = {};
	int i = 0;
	for (auto t = vec.begin(); t != vec.end(); ++t) {
		if (sizeof(std::string) == sizeof(T)) {
			if (vec[i].find(search) != std::string::npos) {
				r.push_back(i);
			}
		}
		else if (*t == search) {
			r.push_back(i);
		}
		i++;
	}
	return r;
}
