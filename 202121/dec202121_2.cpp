//
// Advent of Code 2021, day 21, part two
//

#include <array>
#include <format>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

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

	// Number of outcomes from rolling the quantum die 3 times and
	// adding. For instance 3 occurs in only one world (1,1,1), whereas 8
	// occurs in 3 worlds (2,3,3), (3,2,3), and (3,3,2). These sum up to
	// the 27 (3*3*3) possible new worlds.
	constexpr std::array<int, 10> counts = { 0, 0, 0, 1, 3, 6, 7, 6, 3, 1 };

	// We keep track of the number of worlds where player 1 and player 2
	// have a certain position and score
	std::vector<std::tuple<int, int, int, int, std::size_t>> worlds;

	worlds.emplace_back(p1, 0, p2, 0, 1);

	std::size_t outcomes_1_winning = 0;
	std::size_t outcomes_2_winning = 0;

	while (!worlds.empty()) {
		std::vector<std::tuple<int, int, int, int, std::size_t>> new_worlds;

		for (auto [pos1, score1, pos2, score2, count] : worlds) {
			for (int roll = 3; roll <= 9; ++roll) {
				auto new_pos1 = pos1 + roll > 10 ? pos1 + roll - 10 : pos1 + roll;
				auto new_score1 = score1 + new_pos1;

				if (new_score1 >= 21) {
					outcomes_1_winning += count * counts[roll];
					continue;
				}
				else {
					new_worlds.emplace_back(new_pos1, new_score1, pos2, score2, count * counts[roll]);
				}
			}
		}

		worlds.swap(new_worlds);
		new_worlds.clear();

		for (auto [pos1, score1, pos2, score2, count] : worlds) {
			for (int roll = 3; roll <= 9; ++roll) {
				auto new_pos2 = pos2 + roll > 10 ? pos2 + roll - 10 : pos2 + roll;
				auto new_score2 = score2 + new_pos2;

				if (new_score2 >= 21) {
					outcomes_2_winning += count * counts[roll];
					continue;
				}
				else {
					new_worlds.emplace_back(pos1, score1, new_pos2, new_score2, count * counts[roll]);
				}
			}
		}

		worlds.swap(new_worlds);
	}

	std::cout << std::format("{} total outcomes\n", outcomes_1_winning + outcomes_2_winning);
	std::cout << std::format("{} where player 1 wins\n", outcomes_1_winning);
	std::cout << std::format("{} where player 2 wins\n", outcomes_2_winning);
}
