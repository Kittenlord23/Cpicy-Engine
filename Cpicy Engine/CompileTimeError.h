#pragma once
#include <string>

struct CompileTimeErrorHandel {
	int error_number;
	int line_number;
	std::string line_content;
};
