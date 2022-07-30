#pragma once
#include "vec2.h"

enum struct Direction {
	Up,
	Down,
	Left,
	Right,
	None,
};

struct Camera
{
	int x = 0;
	int y = 0;
	Direction x_direction = Direction::None; 
	Direction y_direction = Direction::None;
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	double time = 0;
	bool in_air = false;
	Camera();
};
