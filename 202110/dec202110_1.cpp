//
// Advent of Code 2021, day 10, part one
//

#include <format>
#include <iostream>
#include <iterator>
#include <numeric>
#include <stack>
#include <string>
#include <string_view>
#include <vector>

long syntax_error_score(std::string_view line)
{
	std::stack<char> stack;

	for (auto ch : line) {
		switch (ch) {
		case '(':
			stack.push(')');
			break;
		case '[':
			stack.push(']');
			break;
		case '{':
			stack.push('}');
			break;
		case '<':
			stack.push('>');
			break;
		case ')':
			if (ch != stack.top()) {
				return 3;
			}
			stack.pop();
			break;
		case ']':
			if (ch != stack.top()) {
				return 57;
			}
			stack.pop();
			break;
		case '}':
			if (ch != stack.top()) {
				return 1197;
			}
			stack.pop();
			break;
		case '>':
			if (ch != stack.top()) {
				return 25137;
			}
			stack.pop();
			break;
		default:
			std::cerr << std::format("unknown char '{}'", ch);
			exit(1);
		}
	}

	return 0;
}

int main()
{
	std::vector<std::string> lines(std::istream_iterator<std::string>{std::cin},
	                               std::istream_iterator<std::string>{});

	std::cout << std::accumulate(lines.begin(), lines.end(), 0L,
		[](long sum, auto &line) {
			return sum + syntax_error_score(line);
		}
	) << '\n';
}
