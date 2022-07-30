#include "RandomEngine.h"

int RandomInt::Result() {
	return output;
}

void RandomInt::Generate(int min, int max) {
	cycle:
	static bool firstTimeFlag = true;
	if (firstTimeFlag == true) {
		firstTimeFlag = false;
		srand((unsigned)time(0));
		goto cycle;
	}
	else {
		output = min + (rand() % (max - min));
	}
}