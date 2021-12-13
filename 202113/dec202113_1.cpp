//
// Advent of Code 2021, day 13, part one
//

#include <algorithm>
#include <format>
#include <iostream>
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

	auto [dir, pos] = folds.front();

	fold(dots, dir, pos);

	std::ranges::sort(dots);

	auto tail = std::ranges::unique(dots);

	dots.erase(tail.begin(), tail.end());

	std::cout << dots.size() << '\n';
}
