#pragma once
#include "Shapes.h"
#include <vector>
#include "Game.h"
#include "Viewer.h"

// TODO: move this to Layer.cpp, unions are scary though

class Game;

struct Content {
	GRect rect;
	GTexture texture;
};
struct Graphic {
	int gtype;
	Content content;
	bool operator < (Graphic other) const {
		int key;
		int other_key;
		if (gtype == 0) {
			key = content.rect.z_index;
		}
		else {
			key = content.texture.z_index;
		}
		if (other.gtype == 0) {
			other_key = other.content.rect.z_index;
		}
		else {
			other_key = other.content.texture.z_index;
		}
		return (key < other_key);
	}
}; 

struct Layer {
	int z_index;
	std::vector<Graphic> chain;
	Layer(int  z_index);
	void AddGraphic(GRect rect);
	void AddGraphic(GTexture texture);
	void Draw(SDL_Renderer* renderer, Game* game);
	void Draw(SDL_Renderer* renderer, Game* game, Camera* editor_camera, Viewer v_drawmode);
	bool operator < (Layer other) const {
		return (z_index < other.z_index);
	}
};
