//
// Advent of Code 2021, day 10, part two
//

#include <algorithm>
#include <format>
#include <iostream>
#include <iterator>
#include <ranges>
#include <stack>
#include <string>
#include <string_view>
#include <vector>

long long completion_score(std::string_view line)
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
			[[fallthrough]];
		case ']':
			[[fallthrough]];
		case '}':
			[[fallthrough]];
		case '>':
			if (ch != stack.top()) {
				return 0;
			}
			stack.pop();
			break;
		default:
			std::cerr << std::format("unknown char '{}'", ch);
			exit(1);
		}
	}

	long long score = 0;

	while (!stack.empty()) {
		switch (stack.top()) {
		case ')':
			score = 5 * score + 1;
			break;
		case ']':
			score = 5 * score + 2;
			break;
		case '}':
			score = 5 * score + 3;
			break;
		case '>':
			score = 5 * score + 4;
			break;
		}

		stack.pop();
	}

	return score;
}

int main()
{
	std::vector<std::string> lines(std::istream_iterator<std::string>{std::cin},
	                               std::istream_iterator<std::string>{});

	auto scores_view = lines
	                 | std::views::transform([](auto &line) { return completion_score(line); })
		         | std::views::filter([](auto score) { return score != 0; });

	std::vector<long long> scores(scores_view.begin(), scores_view.end());

	std::ranges::nth_element(scores, scores.begin() + scores.size() / 2);

	std::cout << scores[scores.size() / 2] << '\n';
}
