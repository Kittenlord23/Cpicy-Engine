#include "CpicyInteger.h"

CpicyInteger::CpicyInteger(std::string name, int value, int scope_level) {
	this->name = name;
	this->value = value;
	this->scope_level = scope_level;
}
void CpicyInteger::ChangeValue(int new_value) {
	this->value = new_value;
}

CpicyIntegerRef::CpicyIntegerRef(std::string name, int* modify, int scope_level) {
	this->name = name;
	this->modify = modify;
}
void CpicyIntegerRef::ChangeValue(int* modify) {
	this->modify = modify;
}

