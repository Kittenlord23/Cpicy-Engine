#pragma once

#include <vector>
#include <iostream>
#include <algorithm>
#include "Editor.h"
#include "Shapes.h"
#include "Game.h"
#include "SDL+.h"
#include "Kiran.h"
#include "Mouse.h"
#include "ScrollBar.h"
#include "Clock.h"
#include "Viewer.h"
#include "Chain.h"
#include "Button.h"
#include "Ribbon.h"


// These are window states that are used to tell
// the application what state the application is in

constexpr auto SPICY_STARTUP = 1;
constexpr auto SPICY_MAIN_MENU = 2;
constexpr auto SPICY_CREATE_NEW_PROJECT = 3;
constexpr auto SPICY_CANVAS = 4;
constexpr auto SPICY_GAME = 5;

class Application {
	SDL_Window* AppWindow = NULL;
	SDL_Surface* AppWindowSurface = NULL;
	SDL_Renderer* AppRenderer = NULL;
	SDL_Event event{};
	const int window_width = GetDesktopResolution('x');
	const int window_height = GetDesktopResolution('y');

	// Images/Textures

	SDL_Surface* Startup480A_Surface = NULL;
	SDL_Texture* QuitButtonTexture = NULL;
	SDL_Texture* CP1_Texture = NULL;
	SDL_Texture* CP2_Texture = NULL;
	SDL_Texture* BP1_Texture = NULL;
	SDL_Texture* SP1_Texture = NULL;
	SDL_Texture* NP1_Texture = NULL;
	SDL_Texture* QPFICON_Texture = NULL;
	SDL_Texture* SVPICON_Texture = NULL;
	SDL_Texture* WINICON_Texture = NULL;
	SDL_Texture* OPFILE_Texture = NULL;
	SDL_Texture* UNICON_Texture = NULL;
	SDL_Texture* REICON_Texture = NULL;
	SDL_Texture* START_Texture = NULL;
	SDL_Texture* CREATIONMODE_Texture = NULL;
	SDL_Texture* GRAPHICSGEAR_Texture = NULL;
	SDL_Texture* IMPORTIMAGE_Texture = NULL;
	SDL_Texture* ADDITEM_Texture = NULL;
	SDL_Texture* IMAGEVIEW_Texture = NULL;
	SDL_Texture* FILEVIEWS_Texture = NULL;
	SDL_Texture* IMAGEVIEWS_Texture = NULL;
	SDL_Texture* CTEXTUREUI_Texture = NULL;
	SDL_Texture* DYNAMICSTATICSTATE_Texture = NULL;
	SDL_Texture* ERASETEXT_Texture = NULL;
	SDL_Texture* ERASEICON_Texture = NULL;
	SDL_Texture* IS_Texture = NULL;
	SDL_Texture* ABOUTICON_Texture = NULL;
	SDL_Texture* HOMETEXT_Texture = NULL;
	SDL_Texture* HOMEICON_Texture = NULL;
	SDL_Texture* DOCUTEXT_Texture = NULL;
	SDL_Texture* DOCUICON_Texture = NULL;
	SDL_Texture* RECTANGLETEXT_Texture = NULL;
	SDL_Texture* RECTANGLEICON_Texture = NULL;
	SDL_Texture* IMAGEICON_Texture = NULL;
	SDL_Texture* IMAGETEXT_Texture = NULL;
	SDL_Texture* RECTWALLTEXT_Texture = NULL;
	SDL_Texture* RECTWALLICON_Texture = NULL;
	SDL_Texture* SWAPDEVMODEICON_Texture = NULL;
	SDL_Texture* COLORPICKERICON_Texture = NULL;
	SDL_Texture* COLORPICKERTEXT_Textrue = NULL;
	SDL_Texture* SPECRECTANGLETEXT_Texture = NULL;
	SDL_Texture* SPECIMAGETEXT_Texture = NULL;
	SDL_Texture* PROJECTTEMPLATETEXT_Texture = NULL;
	SDL_Texture* SIDESCROLLTEXT_Texture = NULL;
	SDL_Texture* SIDESCROLLICON_Texture = NULL;
	SDL_Texture* GODVIEW_Texture = NULL;
	SDL_Texture* ADDLAYER_Texture = NULL;
	SDL_Texture* CURRENTLAYER_Texture = NULL;
	SDL_Texture* REMOVELAYER_Texture = NULL;
	SDL_Texture* ADDLAYERICON_Texture = NULL;
	SDL_Texture* REMOVELAYERICON_Texture = NULL;
	SDL_Texture* LAYERCOUNTER_Texture = NULL;
	SDL_Texture* LAYERUPICON = NULL;
	SDL_Texture* LAYERDOWNICON = NULL;
	SDL_Texture* UPARROW_Texture = NULL;
	SDL_Texture* DOWNARROW_Texture = NULL;

	// Create new project variables

	SDL_Color new_project_text_color = { 255, 255, 255, 255 };
	SDL_Texture* new_project_text_texture = NULL;
	std::string new_project_text = "";
	bool editing_cpn = false;

	// Important flags

	bool should_quit = false;
	int window_state = SPICY_STARTUP;

	// Structers and classes

	Mouse mouse{};
	Game game{};
	Editor editor{};
	Clock clock{};
	Viewer Viewer{};

	// Booleans for keybinds

	bool ctrl_key_pressed = false;
	bool shift_key_pressed = false;
	bool alt_key_pressed = false;

	// Buttons

	Button ConvertButton = Button("Compile", 0, 0, 0, 0, 48, { 55, 55, 60 }, { 50, 150, 50 }, AppRenderer, { 255, 255, 255 });

	// Ribbons

	//RibbonBar ribbonbar;
	std::vector<Ribbon*> ribbons = {};
	Ribbon GeneralSectionRibbon = Ribbon("Home", AppRenderer);
	Ribbon GraphicSectionRibbon = Ribbon("Graphics", AppRenderer);
	Ribbon InteractiveSectionRibbon = Ribbon("Interactive", AppRenderer);
	Ribbon LayerSectionRibbon = Ribbon("Layers", AppRenderer);
	Ribbon EditSectionRibbon = Ribbon("Edit", AppRenderer);

public:
	bool GetQuitState();
	bool initialize();
	void event_handel();
	void loop();
	void TemplateMaker();
	void ColorPicker();
	void close();
	bool LoadMedia();
	// void AboutPage();
	inline Game* GetGamePtr() {
		return &this->game;
	}
	inline Editor* GetEditorPtr() {
		return &this->editor;
	}
	inline SDL_Renderer* GetRendererPtr() {
		return this->AppRenderer;
	}
};