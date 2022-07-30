#include "MathExpression.h"
#include "Game.h"
#include <algorithm>

int EMath::precedence(char op) {
        if (op == '+' || op == '-')
                return 1;
        if (op == '*' || op == '/')
                return 2;
        return 0;
}
int EMath::applyOp(int a, int b, char op) {
        switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        }
}
int EMath::evaluate(std::string tokens) {
        int i;
        std::stack <int> values;
        std::stack <char> ops;

        for (i = 0; i < tokens.length(); i++) {
                if (tokens[i] == ' ')
                        continue;
                else if (tokens[i] == '(') {
                        ops.push(tokens[i]);
                }
                else if (isdigit(tokens[i])) {
                        int val = 0;
                        while (i < tokens.length() &&
                                isdigit(tokens[i]))
                        {
                                val = (val * 10) + (tokens[i] - '0');
                                i++;
                        }

                        values.push(val);
                        i--;
                }
                else if (tokens[i] == ')')
                {
                        while (!ops.empty() && ops.top() != '(')
                        {
                                int val2 = values.top();
                                values.pop();

                                int val1 = values.top();
                                values.pop();

                                char op = ops.top();
                                ops.pop();

                                values.push(applyOp(val1, val2, op));
                        }
                        if (!ops.empty())
                                ops.pop();
                }
                else
                {
                        while (!ops.empty() && precedence(ops.top())
                                >= precedence(tokens[i])) {
                                int val2 = values.top();
                                values.pop();

                                int val1 = values.top();
                                values.pop();

                                char op = ops.top();
                                ops.pop();

                                values.push(applyOp(val1, val2, op));
                        }
                        ops.push(tokens[i]);
                }
        }
        while (!ops.empty()) {
                int val2 = values.top();
                values.pop();

                int val1 = values.top();
                values.pop();

                char op = ops.top();
                ops.pop();

                values.push(applyOp(val1, val2, op));
        }
        return values.top();
}

int evaluateIndexerj(std::string tokens, Game* game) {
        return 0;
}

int EMath::evaluateMath(std::string input, Game* game) {
        std::string out = input;
        for (auto instance = game->instances.begin(); instance != game->instances.end(); ++instance) {
                for (auto field = instance->fields.begin(); field != instance->fields.end(); ++field) {
                        out = replace(out, instance->identifier + "." + field->identifier, std::to_string(field->value));
                }
        }
        for (int x = game->integers.size() - 1; x >= 0; x--) {
                out = replace(replace(out, "<integer>", ""), game->integers[x].name, std::to_string(game->integers[x].value));
        }
        for (auto& var : game->integer_pointers) {
                out = replace(out, replace(var.name, "<&integer>", ""), std::to_string(*var.modify));
        }
        out += "+0";
        out = std::to_string(EMath::evaluate(out));
        return std::stoi(out);
}