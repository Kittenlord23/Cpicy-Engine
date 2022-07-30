// Loads media for the application
// CPU GPU and LoadText are all SDL+.h functions which are used for loading images and text
// in a more streamlined way then having to set up the pointers and everything

// Included for the prototypes

#include "App.h"

// Media function, one time call

bool Application::LoadMedia() {
	/*
		Loaded media that is avalible in the application scope

		All names are abbreviated otherwise they would be really long, but quit reference :
		* CP = Create project
		* BP = Back project
		* SP1 = Select project
		* SP = Save project, a little confusing but it works
		* F and File is for the File tab
		* Items trailing in ICON are loaded icons, each one can be deduced by looking at the path name

		The local variable for the icon is the icon of the entire window, and it only needs to be set once,
		so it is local instead of being global or a attribute
	*/
	Startup480A_Surface = CPU_LoadSurface("Extern\\Assets\\Images\\CPU\\Images\\Startup480A.png", AppWindowSurface);
	QuitButtonTexture = GPU_LoadTexture("Extern\\Assets\\Images\\GPU\\Images\\QuitButton.png", AppRenderer);
	CP1_Texture = GPU_LoadText("Create New Project", AppRenderer, 42,GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	CP2_Texture = GPU_LoadText("Create", AppRenderer, 42,GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	BP1_Texture = GPU_LoadText("Back", AppRenderer, 42,GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	SP1_Texture = GPU_LoadText("Select Project", AppRenderer, 42,GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	NP1_Texture = GPU_LoadText("Project Name:", AppRenderer, 42,GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	WINICON_Texture = GPU_LoadTexture("Extern\\Assets\\Images\\CPU\\Images\\WindowIcon32.png", AppRenderer);
	SVPICON_Texture = GPU_LoadTexture("Extern\\Assets\\Images\\GPU\\Images\\SaveIcon.png", AppRenderer);
	OPFILE_Texture = GPU_LoadTexture("Extern\\Assets\\Images\\GPU\\Images\\OpenFileIcon.png", AppRenderer);
	QPFICON_Texture = GPU_LoadTexture("Extern\\Assets\\Images\\GPU\\Images\\CloseFileIcon.png", AppRenderer);
	UNICON_Texture = GPU_LoadTexture("Extern\\Assets\\Images\\GPU\\Images\\UndoIcon.png", AppRenderer);
	REICON_Texture = GPU_LoadTexture("Extern\\Assets\\Images\\GPU\\Images\\RedoIcon.png", AppRenderer);
	START_Texture = GPU_LoadText("Preview", AppRenderer, floor(GetDesktopResolution('y') / 50),GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	GRAPHICSGEAR_Texture = GPU_LoadTexture("Extern\\Assets\\Images\\GPU\\Images\\GraphicsSettingsIcon300.png", AppRenderer);
	IMPORTIMAGE_Texture = GPU_LoadText("Import Image", AppRenderer, floor(GetDesktopResolution('y') / 77),GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	ADDITEM_Texture = GPU_LoadText("Add Item", AppRenderer, floor(GetDesktopResolution('y') / 60),GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	IMAGEVIEW_Texture = GPU_LoadText("File View", AppRenderer, floor(GetDesktopResolution('y') / 60),GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	FILEVIEWS_Texture = GPU_LoadText("Spice", AppRenderer, floor(GetDesktopResolution('y') / 77),GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	IMAGEVIEWS_Texture = GPU_LoadText("Images", AppRenderer, floor(GetDesktopResolution('y') / 77),GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	CTEXTUREUI_Texture = GPU_LoadText("Current Image: None", AppRenderer, floor(GetDesktopResolution('y') / 50),GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	DYNAMICSTATICSTATE_Texture = GPU_LoadText("Moves With Camera: NO", AppRenderer, floor(GetDesktopResolution('y') / 50),GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	ERASETEXT_Texture = GPU_LoadText("Erase Item                                   (Ctrl + DEL)", AppRenderer, floor(GetDesktopResolution('y') / 77),GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	ABOUTICON_Texture = GPU_LoadTexture(GetExecutablePath() + "Extern\\Assets\\Images\\GPU\\Images\\AboutIcon.png", AppRenderer);
	ERASEICON_Texture = GPU_LoadTexture("Extern\\Assets\\Images\\GPU\\Images\\eraserIcon.png", AppRenderer);
	IS_Texture = GPU_LoadText("         Environment         ", AppRenderer, 42,GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	HOMETEXT_Texture = GPU_LoadText("Main Menu", AppRenderer, 15, GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	HOMEICON_Texture = GPU_LoadTexture(GetExecutablePath() + "Extern\\Assets\\Images\\GPU\\Images\\homeicon.png", AppRenderer);
	DOCUTEXT_Texture = GPU_LoadText("Documentation", AppRenderer, 15, GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	DOCUICON_Texture = GPU_LoadTexture(GetExecutablePath() + "Extern\\Assets\\Images\\GPU\\Images\\docupageicon.png", AppRenderer);
	RECTANGLETEXT_Texture = GPU_LoadText("Rectangle", AppRenderer, 15, GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	RECTANGLEICON_Texture = GPU_LoadTexture(GetExecutablePath() + "Extern\\Assets\\Images\\GPU\\Images\\Rectangleicon.png", AppRenderer);
	IMAGETEXT_Texture = GPU_LoadText("Image", AppRenderer, 15, GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	IMAGEICON_Texture = GPU_LoadTexture(GetExecutablePath() + "Extern\\Assets\\Images\\GPU\\Images\\Imageicon.png", AppRenderer);
	RECTWALLTEXT_Texture = GPU_LoadText("Wall", AppRenderer, 15, GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	RECTWALLICON_Texture = GPU_LoadTexture(GetExecutablePath() + "Extern\\Assets\\Images\\GPU\\Images\\rectwallicon.png", AppRenderer);
	SWAPDEVMODEICON_Texture = GPU_LoadTexture(GetExecutablePath() + "Extern\\Assets\\Images\\GPU\\Images\\swapdevmodeicon.png", AppRenderer);
	COLORPICKERICON_Texture = GPU_LoadTexture(GetExecutablePath() + "Extern\\Assets\\Images\\GPU\\Images\\colorpickericon.png", AppRenderer);
	COLORPICKERTEXT_Textrue = GPU_LoadText("Color Picker", AppRenderer, 15, GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	SPECRECTANGLETEXT_Texture = GPU_LoadText("Cord Rect", AppRenderer, 15, GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	SPECIMAGETEXT_Texture = GPU_LoadText("Cord Image", AppRenderer, 15, GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	PROJECTTEMPLATETEXT_Texture = GPU_LoadText("Default Code Generation", AppRenderer, 20, GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	SIDESCROLLTEXT_Texture = GPU_LoadText("Side Scroller", AppRenderer, 15, GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	SIDESCROLLICON_Texture = GPU_LoadTexture(GetExecutablePath() + "Extern\\Assets\\Images\\GPU\\Images\\sidescroll.png", AppRenderer);
	CURRENTLAYER_Texture = GPU_LoadText("Current Layer: 0", AppRenderer, floor(GetDesktopResolution('y') / 50), GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	ADDLAYER_Texture = GPU_LoadText("Add Layer", AppRenderer, 15, GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	REMOVELAYER_Texture = GPU_LoadText("Remove Layer", AppRenderer, 15, GetExecutablePath() + "Extern\\Fonts\\arial.ttf");
	REMOVELAYERICON_Texture = GPU_LoadTexture(GetExecutablePath() + "Extern\\Assets\\Images\\GPU\\Images\\RemoveLayerIcon.png", AppRenderer);
	ADDLAYERICON_Texture = GPU_LoadTexture(GetExecutablePath() + "Extern\\Assets\\Images\\GPU\\Images\\AddLayerIcon.png", AppRenderer);
	UPARROW_Texture = GPU_LoadTexture(GetExecutablePath() + "Extern\\Assets\\Images\\GPU\\Images\\UpArrow.png", AppRenderer);
	DOWNARROW_Texture = GPU_LoadTexture(GetExecutablePath() + "Extern\\Assets\\Images\\GPU\\Images\\DownArrow.png", AppRenderer);
	ConvertButton.InitializeButton(AppRenderer);
	SDL_Surface* Icon = CPU_LoadSurface("Extern\\Assets\\Images\\CPU\\Images\\WindowIcon32.png", AppWindowSurface);							// Create icon
	SDL_SetWindowIcon(AppWindow, Icon);																					// Set icon
	SDL_FreeSurface(Icon);																								// Free the icon surface
	Icon = NULL;																										// Point it to null
	// At one point it would reutrn false if something went wrong, but that is apparently taken care of
	// somewhere else in the program

	return true;
}