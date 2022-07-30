#pragma once
#include <string>
#include <vector>
#include "Chain.h"

struct UserFunction
{
	std::string name;
	std::vector<std::string> arg_names;
	std::vector<std::string> commands;
};

