#pragma once
#include "DynamicClass.h"
#include "CpicyInteger.h"
struct CpicyVector1D_USER
{
	std::string name;
	std::vector<UserInstance> vec;
	int scope_level;

	CpicyVector1D_USER();
};

struct CpicyVector1D_INTEGER {
	std::string name;
	std::vector<CpicyInteger> vec;
	int scope_level;

	CpicyVector1D_INTEGER(const std::string &name, int scope_level, std::string content);
};
