#pragma once
#include <random>
#include <time.h>

class RandomInt {
	int output;

public:
	int Result();
	void Generate(int min, int max);
};