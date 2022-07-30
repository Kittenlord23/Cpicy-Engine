#include "App.h"

int main(int argc, char** argv) {
	Application app;
	if (app.initialize()) {
		if (app.LoadMedia()) {
			while (!app.GetQuitState()) {
				app.loop();
			}
		}
		else {
			std::cout << "app failed to load media";
		}
	}
	else {
		std::cout << "app failed to initialize"; 
	}
	return 0;
}