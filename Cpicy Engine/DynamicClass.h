#pragma once
#include <vector>
#include <string>

enum struct DynamicClassExcpetionCode {
	ShortNameError = 0,
	NoClassNameError = 1,
	NoFieldNameError = 2,
};

struct DynamicClassException {
	DynamicClassExcpetionCode e;
};

struct UserClassInteger {
	friend class UserClass;
	friend class UserInstance;
	int value;
	std::string identifier;

	UserClassInteger(std::string identifier, int value);
	void SetValue(int value) noexcept;
	inline int GetValue() const noexcept;
};

class UserClass {

public:
	std::string name; // Name of the class
	std::string identifier; // Name of an *instance* of a class

private:
	std::vector<UserClassInteger> built_in_fields;
private:

	friend class UserInstance; // Allows use of UserClass::UserClass in UserInstance
	UserClass();

public:

	// Construct new class for adding types

	UserClass(const std::string& name);

	// Add custom type

	void AddField(UserClass field, std::string identifier);

	void AddInteger(UserClassInteger integer) noexcept;

	void AddInteger(const std::string name, const int value);
};

class UserInstance {
public:
	std::vector<UserClassInteger> fields;
	std::string identifier;
	int scope_level;
public:
	UserInstance(std::string class_name, std::string identifier, const std::vector<UserClass>& blueprints, int scope_level);
	int LookupField(const std::string& search) const;
	void SetField(const std::string& search, int value);
};

struct UserInstanceRef {
	UserInstance* ptr;
	std::string identifier;
	int scope_level;
};