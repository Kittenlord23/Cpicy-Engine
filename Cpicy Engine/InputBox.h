#pragma once
#include <iostream>
#include <SDL.h>

class InputBox
{
	std::string return_value = "";
public:
	InputBox(int width, int height, std::string title, std::string poll, int max_reponse_length, bool allow_cancel, bool number_only, bool allow_data_macro, std::string cancel_response = "");
	
	std::string GetInputFromBox();
};

