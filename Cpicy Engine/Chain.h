#pragma once
#include <SDL.h>
#include <vector>
#include <string>
#include <iostream>
#include "Clock.h"

class Game;

struct TriggerTimer {
	double dest;
	double current;
};
union CauseValue {
	SDL_KeyCode key;
	SDL_Rect clicked_on;
	TriggerTimer timer;
	bool unused;
	CauseValue();
	~CauseValue() {
		
	}
};

struct TriggerType {
	std::string cause;
	CauseValue cause_value;
};

struct Link {
	TriggerType trigger;
	std::string action;
	std::string send_data;
	bool push;
	int unique_id = -1;
};

struct Chain {
	std::vector<Link> chain;
	std::string current_data;
	void InterpretChain(SDL_Event e, Game* game, Clock c, SDL_Renderer* renderer);
private:
	std::string RunAction(Link* link, std::string incoming_data, Game* game, SDL_Renderer* renderer, int recersion_count);
};

