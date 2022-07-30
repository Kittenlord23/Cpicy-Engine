// CPP program to evaluate a given
// expression where tokens are
// separated by space.
#include <iostream>
#include <vector>
#include <stack>
#include "Game.h"

namespace EMath {
        int precedence(char op);
        int applyOp(int a, int b, char op);
        int evaluate(std::string tokens);
        int evaluateMath(std::string input, Game* game);
}
// This code is contributed by Nikhil jindal and altered.