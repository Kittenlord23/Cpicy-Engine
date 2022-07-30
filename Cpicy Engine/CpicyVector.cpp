#include "CpicyVector.h"
#include "Kiran.h"

CpicyVector1D_USER::CpicyVector1D_USER() {

}

CpicyVector1D_INTEGER::CpicyVector1D_INTEGER(const std::string &name, int scope_level, std::string content) {
	this->name = name;
	this->scope_level = scope_level;
	content = replace(replace(replace(content, " ", ""), "[", ""), "]", "");
	for (auto value : split(content, ",")) {
		CpicyInteger c = CpicyInteger("", std::stoi(value), -1);
		vec.push_back(c);
	}
}