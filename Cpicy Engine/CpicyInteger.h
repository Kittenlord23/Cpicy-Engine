#pragma once
#include <iostream>

struct CpicyInteger
{
	std::string name;
	int value;
	int scope_level;
	CpicyInteger(std::string name, int value, int scope_level);
	void ChangeValue(int new_value);
};

struct CpicyIntegerRef {
	std::string name;
	int* modify;
	int scope_level;
	CpicyIntegerRef(std::string name, int* value, int scope_level);
	void ChangeValue(int* new_value);
};
