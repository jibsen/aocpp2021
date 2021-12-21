//
// Advent of Code 2021, day 21, part one
//

#include <format>
#include <iostream>
#include <string>

std::pair<int, int> read_starting_positions()
{
	std::string line1, line2;

	std::getline(std::cin, line1);
	std::getline(std::cin, line2);

	return {std::stoi(line1.substr(line1.size() - 2)),
	        std::stoi(line2.substr(line2.size() - 2))};
}

int main()
{
	auto [p1, p2] = read_starting_positions();

	std::cout << std::format("starting positions {}, {}\n", p1, p2);

	int score1 = 0;
	int score2 = 0;

	int die = 1;

	int rolls = 0;

	for (;;) {
		for (int i = 0; i < 3; ++i) {
			p1 += die;
			if (p1 > 10) {
				p1 = ((p1 - 1) % 10) + 1;
			}
			die = die == 100 ? 1 : die + 1;
		}

		score1 += p1;

		rolls += 3;

		if (score1 >= 1000) {
			std::cout << score2 * rolls;
			break;
		}

		for (int i = 0; i < 3; ++i) {
			p2 += die;
			if (p2 > 10) {
				p2 = ((p2 - 1) % 10) + 1;
			}
			die = die == 100 ? 1 : die + 1;
		}

		score2 += p2;

		rolls += 3;

		if (score2 >= 1000) {
			std::cout << score1 * rolls;
			break;
		}
	}
}
