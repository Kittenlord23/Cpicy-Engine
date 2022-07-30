#pragma once
#include <vector>
#include <iostream>
#include "Game.h"

struct Editor;

class CommandQueue
{
	std::vector<std::string> commands;

private:
	void evaluate_expression(Game* game, Editor* editor);

public:
	void push_queue(std::string in);
	void flush_queue(Game* game, Editor* editor);
	void pop_queue();
	CommandQueue();
};

