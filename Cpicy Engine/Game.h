#pragma once
#include "Shapes.h"
#include "Camera.h"
#include <string>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include "Kiran.h"
#include <vector>
#include <SDL.h>
#include "Shapes.h"
#include "Texture.h"
#include "CpicyInteger.h"
#include "UserFunction.h"
#include "Layer.h"
#include "DynamicClass.h"
#include "CpicyVector.h"
class Layer;
class Chain;
struct TriggerType;
constexpr auto GLOBAL_SCOPE = -1;
constexpr auto FIELD_SCOPE = -2;
class Game {
	std::string current_working_path;
	std::string game_template;
	void loadLogicFile(std::string path);
	void convertStringToLogic(std::string str);
public:
	bool using_degrees = false;
	int gravity_force = 10;
	int jump_power = 10;
	Camera game_camera;
	float camera_speed = 1.0f;
	bool enable_gravity = false;
	int max_frame_rate = 0;
	bool enable_errors = false;
	std::vector<GRect> cp_rect_data;
	std::vector<LoadedTexture> loaded_textures = {};
	std::vector<GTexture> cp_texture_data = {};
	std::vector<GRWall> cp_rect_wall_data = {};
	std::vector<Chain> cp_EChain_data = {};
	std::vector<Chain> cp_LChain_data = {};
	Chain start_chain = {};
	SDL_Color user_code_draw_color = {255, 255, 255};
	std::vector<CpicyInteger> integers = {};
	std::vector<std::vector<CpicyInteger>> integer_arrays = {};
	std::vector<CpicyIntegerRef> integer_pointers = {};
	std::vector<UserFunction> functions = {};
	std::vector<Layer> layers = {};
	std::vector<UserClass> classes = {};
	std::vector<UserInstance> instances = {};
	std::vector<UserInstanceRef> instance_referances = {};
	// To remove
	std::string data1;
	std::string data2;
	std::string data3;
	std::string data4;
	std::string data5;
	// Variables that are created must be removed when the go out of scope. 
	// Closing a scope removes the top x integers off the interger_data where
	// x is the value of void_stack.back(). Then pop_back void_stack
	std::vector<int> void_integer_pointers = {};
	std::vector<int> void_integers = {};
	std::vector<int> void_userclass_pointers = {};
	std::vector<int> void_userclasses = {};
	int recersion_count = 0;
	int function_call_stack_count = 0;
	std::string BoolStr(bool boolean);
	CauseValue GetCauseValueFromString(std::string cause, std::string input);
	std::string getTriggerCauseAsString(TriggerType trigger);
	void setData(std::string command);
	void logError(std::string link_command, std::string description, std::string example);
	void clear();
	std::string getWorkingPath();
	void saveProject();
	void loadTextures(std::string path, SDL_Renderer* renderer);
	void addRect(GRect);
	void addTexture(GTexture);
	void scope();
	void unscope();
	bool setWorkingPath(std::string path, SDL_Renderer* renderer);
	std::string formatDataString(std::string command);
	void Sort();
	inline double to_degrees(double x) {
		if (using_degrees) {
			double y = M_PI * (x / 180);
			return y;
		}
		return x;
	}
	int LookUpVariable(const std::string& search);
};
