#include "Layer.h"
#include "Viewer.h"

Layer::Layer(int z_index) {
	this->z_index = z_index;
}
void Layer::AddGraphic(GRect rect) {
	Graphic g;
	g.content.rect = rect;
	g.gtype = 0; // 0 is rectange || TODO: Make enum for types, or #define if lazy
	this->chain.push_back(g);
}
void Layer::AddGraphic(GTexture texture) {
	Graphic g;
	g.content.texture = texture;
	g.gtype = 1; // 1 is texture
	this->chain.push_back(g);
}
void Layer::Draw(SDL_Renderer* renderer, Game* game) {
	for (auto i = chain.begin(); i != chain.end(); ++i) {
		if (i->gtype == 0) {
			if (i->content.rect.dynamic_static == 0) {
				SDL_Rect rect;
				rect.x = i->content.rect.x;
				rect.y = i->content.rect.y;
				rect.w = i->content.rect.w;
				rect.h = i->content.rect.h;
				SDL_SetRenderDrawColor(renderer, i->content.rect.r, i->content.rect.g, i->content.rect.b, 255);
				SDL_RenderFillRect(renderer, &rect);
			}
			else {
				SDL_Rect rect;
				rect.x = i->content.rect.x + game->game_camera.x;
				rect.y = i->content.rect.y + game->game_camera.y;
				rect.w = i->content.rect.w;
				rect.h = i->content.rect.h;
				SDL_SetRenderDrawColor(renderer, i->content.rect.r, i->content.rect.g, i->content.rect.b, 255);
				SDL_RenderFillRect(renderer, &rect);
			}
		}
		if (i->gtype == 1) {
			if (i->content.texture.dynamic_static == 0) {
				SDL_Rect rect;
				rect.x = i->content.texture.x;
				rect.y = i->content.texture.y;
				rect.w = i->content.texture.w;
				rect.h = i->content.texture.h;
				for (auto texture = game->loaded_textures.begin(); texture != game->loaded_textures.end(); ++texture) {
					if (texture->name == i->content.texture.texture.name) {
						SDL_RenderCopyEx(renderer, texture->content, NULL, &rect, i->content.texture.rotation_angle, NULL, SDL_FLIP_NONE);
					}
				};
			}
			else {
				SDL_Rect rect;
				rect.x = i->content.texture.x + game->game_camera.x;
				rect.y = i->content.texture.y + game->game_camera.y;
				rect.w = i->content.texture.w;
				rect.h = i->content.texture.h;
				for (auto texture = game->loaded_textures.begin(); texture != game->loaded_textures.end(); ++texture) {
					if (texture->name == i->content.texture.texture.name) {
						SDL_RenderCopyEx(renderer, texture->content, NULL, &rect, i->content.texture.rotation_angle, NULL, SDL_FLIP_NONE);
						break;
					}
				}
			}
		}
	}
}

void Layer::Draw(SDL_Renderer* renderer, Game* game, Camera* editor_camera, Viewer v_drawmode) {
	for (auto i = chain.begin(); i != chain.end(); ++i) {
		if (i->gtype == 0) {
			if (v_drawmode.rectangles) {
				SDL_Rect rect;
				rect.x = i->content.rect.x + editor_camera->x + GetDesktopResolution('x') * 0.15;
				rect.y = i->content.rect.y + editor_camera->y + GetDesktopResolution('y') * 0.2;
				rect.w = i->content.rect.w;
				rect.h = i->content.rect.h;
				SDL_SetRenderDrawColor(renderer, i->content.rect.r, i->content.rect.g, i->content.rect.b, 255);
				SDL_RenderFillRect(renderer, &rect);
			}
		}
		if (i->gtype == 1) {
			if (v_drawmode.textures) {
				SDL_Rect rect;
				rect.x = i->content.texture.x + editor_camera->x + GetDesktopResolution('x') * 0.15;
				rect.y = i->content.texture.y + editor_camera->y + GetDesktopResolution('y') * 0.2;
				rect.w = i->content.texture.w;
				rect.h = i->content.texture.h;
				for (auto texture = game->loaded_textures.begin(); texture != game->loaded_textures.end(); ++texture) {
					if (texture->name == i->content.texture.texture.name) {
						SDL_RenderCopyEx(renderer, texture->content, NULL, &rect, i->content.texture.rotation_angle, NULL, SDL_FLIP_NONE);
					}
				}
			}
		}
	}
}