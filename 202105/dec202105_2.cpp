//
// Advent of Code 2021, day 5, part two
//

#include <algorithm>
#include <cstdlib>
#include <format>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>

struct Line {
	int x1, y1;
	int x2, y2;
};

auto read_lines()
{
	std::vector<Line> lines;

	std::string arrow;
	char sep;

	Line line;

	while (std::cin >> line.x1 >> sep >> line.y1 >> arrow >> line.x2 >> sep >> line.y2) {
		lines.push_back(line);
	}

	return lines;
}

int main()
{
	auto lines = read_lines();

	std::vector<std::vector<int>> map;

	map.reserve(1000);

	for (int i = 0; i < 1000; ++i) {
		map.emplace_back(1000, 0);
	}

	std::cout << std::format("{} lines read\n", lines.size());

	for (auto &line : lines) {
		int steps = std::max(std::abs(line.x2 - line.x1), std::abs(line.y2 - line.y1));

		int delta_x = line.x1 == line.x2 ? 0
		            : line.x1 < line.x2 ? 1
		            : -1;
		int delta_y = line.y1 == line.y2 ? 0
		            : line.y1 < line.y2 ? 1
		            : -1;

		int x = line.x1;
		int y = line.y1;

		for (int i = 0; i <= steps; ++i) {
			map[y][x]++;

			x += delta_x;
			y += delta_y;
		}
	}

	std::cout << std::accumulate(map.begin(), map.end(), 0,
		[](auto sum, auto &row) {
			return sum + std::ranges::count_if(row, [](auto n) { return n > 1; });
		}
	) << '\n';
}
