//
// Advent of Code 2021, day 13, part two
//

#include <algorithm>
#include <format>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

enum class Direction {
	up, left
};

auto read_dots()
{
	std::vector<std::pair<int, int>> dots;

	for (std::string line; std::getline(std::cin, line) && !line.empty(); ) {
		std::istringstream ss(line);

		int x = 0;
		int y = 0;
		char ch = '\0';

		ss >> x >> ch >> y;

		dots.emplace_back(x, y);
	}

	return dots;
}

auto read_folds()
{
	std::vector<std::pair<Direction, int>> folds;

	for (std::string line; std::getline(std::cin, line); ) {
		folds.emplace_back(line[11] == 'x' ? Direction::left : Direction::up,
		                   std::stoi(line.substr(13)));
	}

	return folds;
}

void fold(std::vector<std::pair<int, int>> &dots, Direction dir, int pos)
{
	switch (dir) {
	case Direction::up:
		for (auto &[x, y] : dots) {
			if (y > pos) {
				y = pos - (y - pos);
			}
		}
		break;
	case Direction::left:
		for (auto &[x, y] : dots) {
			if (x > pos) {
				x = pos - (x - pos);
			}
		}
		break;
	}
}

int main()
{
	auto dots = read_dots();

	auto folds = read_folds();

	std::cout << std::format("{} dots read\n", dots.size());
	std::cout << std::format("{} folds read\n", folds.size());

	int min_fold_x = std::numeric_limits<int>::max();
	int min_fold_y = std::numeric_limits<int>::max();

	for (auto [dir, pos] : folds) {
		fold(dots, dir, pos);

		if (dir == Direction::up) {
			min_fold_y = std::min(min_fold_y, pos);
		}
		else {
			min_fold_x = std::min(min_fold_x, pos);
		}
	}

	std::ranges::sort(dots);

	auto tail = std::ranges::unique(dots);

	dots.erase(tail.begin(), tail.end());

	for (int y = 0; y < min_fold_y; ++y) {
		for (int x = 0; x < min_fold_x; ++x) {
			if (std::ranges::binary_search(dots, std::pair(x, y))) {
				std::cout << '#';
			}
			else {
				std::cout << '.';
			}
		}
		std::cout << '\n';
	}
}
