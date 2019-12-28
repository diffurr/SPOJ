//ONP - Transform the Expression
//using Dijkstra's shunting-yard algorithm
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <stack>

enum token_type { BAD, OPERAND, OPERATOR, LBRAC, RBRAC};

token_type check_token(const char c)
{
	if (c >= 'a' && c <= 'z') return OPERAND;
	if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') return OPERATOR;
	if (c == '(') return LBRAC;
	if (c == ')') return RBRAC;
	return BAD;
}

int check_precedence(const char c)
{
	switch (c) {
	case '-':
	case '+':
		return 2;
	case '/':
	case '*':
		return 3;
	case '^':
		return 4;
	default:
		return 0;
	}
}

void transform(std::string &str)
{
	std::stringstream output;
	std::stack<char> stack;
	char top;

	for (char c : str) {
		token_type token = check_token(c);
		if (token == OPERAND) {
			output << c;
		}
		else if (token == OPERATOR || token == LBRAC || token == RBRAC) {
			if (token == LBRAC) {
				stack.push(c);
			}
			else if (token == RBRAC) {
				do {
					top = stack.top();
					stack.pop();
					if (top != '(') {
						output << top;
					}
				} while (top != '(' && !stack.empty());
			}
			else if (token == OPERATOR) {
				top = stack.top();
				while (check_token(top) == OPERATOR && (check_precedence(top) >= check_precedence(c)) && !stack.empty()) {
					output << top;
					stack.pop();
					top = stack.top();
				}
				stack.push(c);
			}
		}
	}

	while (!stack.empty()) {
		output << stack.top();
		stack.pop();
	}

	str = output.str();
}

int main()
{
	std::vector<std::string> input;
	int nul;
	std::cin >> nul;
	std::string s;
	while (std::cin >> s) {
		input.push_back(s);
	}

	std::for_each(input.begin(), input.end(), transform);

	for (auto s : input) {
		if (!s.empty()) {
			std::cout << s << "\n";
		}
	}
		
	return 0;
}