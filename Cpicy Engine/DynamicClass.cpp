#include "DynamicClass.h"
#include <iostream>

UserClassInteger::UserClassInteger(std::string name, int value) {
	this->identifier = name;
	this->value = value;
}
void UserClassInteger::SetValue(int value) noexcept {
	this->value = value;
}
inline int UserClassInteger::GetValue() const noexcept {
	return this->value;
}
UserClass::UserClass() {

}
UserClass::UserClass(const std::string& name) {
	if (name.size() <= 0) {
		throw DynamicClassException{ DynamicClassExcpetionCode::ShortNameError };
	}
	this->name = name;
} 
void UserClass::AddField(UserClass field, std::string identifier) {
	if (identifier.size() <= 0) {
		throw DynamicClassException{ DynamicClassExcpetionCode::ShortNameError };
	}
	for (int x = 0; x < field.built_in_fields.size(); x++) {
		built_in_fields.push_back(UserClassInteger{ "." + identifier + field.built_in_fields[x].identifier, field.built_in_fields[x].value});
	}
}
void UserClass::AddInteger(UserClassInteger integer) noexcept {
	built_in_fields.push_back(integer);
}
void UserClass::AddInteger(const std::string name, const int value) {
	if (name.size() <= 0) {
		throw DynamicClassException{ DynamicClassExcpetionCode::ShortNameError };
	}
	built_in_fields.push_back(UserClassInteger{ name, value });
}

UserInstance::UserInstance(std::string class_name, std::string identifier, const std::vector<UserClass>& blueprints, int scope_level) {
	this->identifier = identifier;
	this->scope_level = scope_level;
	for (int x = 0; x < blueprints.size() ; x++) {
		if (blueprints[x].name == class_name) {
			fields = blueprints[x].built_in_fields;
			return;
		}
	}
	throw DynamicClassException{ DynamicClassExcpetionCode::NoClassNameError };
}
int UserInstance::LookupField(const std::string& search) const {
	for (auto field = fields.begin(); field != fields.end(); ++field) {
		if (field->identifier == search) {
			return field->value;
		}
	}
	throw DynamicClassException{ DynamicClassExcpetionCode::NoFieldNameError };
}
void UserInstance::SetField(const std::string& search, int value) {
	for (auto field = fields.begin(); field != fields.end(); ++field) {
		if (field->identifier == search) {
			field->value = value;
			return;
		}
	}
	throw DynamicClassException{ DynamicClassExcpetionCode::NoFieldNameError };
}
