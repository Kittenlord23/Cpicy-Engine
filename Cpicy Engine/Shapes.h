#pragma once
#include <SDL.h>
#include "Texture.h"

struct GRect
{
	int x;									// X cord
	int y;									// Y cord
	int w;								// Width
	int h;									// Height
	int r;
	int g;
	int b;
	bool dynamic_static;
	std::string id;
	int layer_number;
	int z_index; // Location on the vector
	bool operator==(const GRect &right) const {
		if ((this->x == right.x) && (this->y == right.y) && (this->w == right.w) && (this->r == right.r) && (this->g == right.g) && (this->b == right.b) && (this->dynamic_static == right.dynamic_static) && (this->id == right.id) && (this->layer_number == right.layer_number)) {
			return true;
		}
		else {
			return false;
		}
	}
};
// Defines a point on a 2d plane
struct Point {
	int x;
	int y;
	std::string id;
};

struct GTexture {
	int x;
	int y;
	int w;
	int h;
	int rotation_angle;
	bool dynamic_static;
	Texture texture;
	std::string id;
	int layer_number;
	int z_index; // Location on the vector
};

struct GRWall {
	int x;
	int y;
	int w;
	int h;
	bool dynamic_static;
	inline bool operator==(GRWall other) {
		if (this->x == other.x && this->y == other.y && this->w == other.w && this->h == other.h && this->dynamic_static == other.dynamic_static)
			return true;
		else
			return false;
	}
	std::string id;
};